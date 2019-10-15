#include "SelectLevelState.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include "../../model/utils/FileUtils.h"
#include "StateManager.h"
#include "../../model/utils/FileDialogUtils.h"
#include "../../model/LevelBuilder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <zip.h>

using namespace boost::filesystem;

namespace vc {
	const std::string SelectLevelState::WORLD_SAVE_PATH = getApplicationFolder().append("\\saves");


	SelectLevelState::SelectLevelState(StateManager& stateManager, egui::MasterRenderer& renderer, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool) :
			State(stateManager),
			vaoCreator(vaoCreator),
			threadPool(threadPool),
			renderer(renderer) {

		updateLevelMetadataArr();
		

		this->elem = std::shared_ptr<egui::SelectLevelElement>(new egui::SelectLevelElement(
			levelMetadataArr,

			getLambda_selectFunc(),
			getLambda_deleteFunc(),
			getLambda_createFunc(),
			getLambda_exportFunc(),
			getLambda_importFunc()
		));
		elem->setOnBackBtn({[this](egui::ActionEvent&) {
			getStateMananger().changeState("MainMenuState");
		}});
		this->scene.setRootElement(elem);
	}

	SelectLevelState::~SelectLevelState() {
		getStateMananger().setCurrentLevel(nullptr);
	}


	egui::selectLevelFunct SelectLevelState::getLambda_selectFunc() {
		return [this](std::shared_ptr<LevelMetadata> metadata) {
			Level* p_l = LevelBuilder::loadLevel(metadata, this->vaoCreator, this->threadPool);

			getStateMananger().setCurrentLevel(p_l);
			getStateMananger().changeState("IngameState");
		};
	}

	egui::deleteLevelFunct SelectLevelState::getLambda_deleteFunc() {
		return [this](std::shared_ptr<LevelMetadata> metadata) {
			LevelBuilder::deleteLevel(metadata->getLevelFolderPath());
			updateLevelMetadataArr();
		};
	}

	egui::createLevelFunct SelectLevelState::getLambda_createFunc() {
		return [this](std::string levelName, std::string seedString) {

			int counter = 0;
			std::string fileName = generateLevelFolderName(levelName);
			Level* p_l = LevelBuilder::createLevel(WORLD_SAVE_PATH + "\\" + fileName, levelName, seedString, this->vaoCreator, this->threadPool);

			updateLevelMetadataArr();
			getStateMananger().setCurrentLevel(p_l);
			getStateMananger().changeState("IngameState");
		};
	}

	egui::exportFunct SelectLevelState::getLambda_exportFunc() {
		return [this](std::shared_ptr<LevelMetadata> metadata) {
			// open save dialog
			std::optional<std::string> fileName = openLevelExportDialog();
			if(!fileName.has_value()) return;

			// create zip file on disk
			int error = 0;
			zip* archive = zip_open(fileName.value().c_str(), ZIP_CREATE, &error);
			if(archive == nullptr) {
				zip_error_t ziperror;
				zip_error_init_with_code(&ziperror, error);
				throw std::runtime_error("Failed to open output file " + fileName.value() + ": " + zip_error_strerror(&ziperror));
			}

			try {
				std::string levelFolderPath = metadata->getLevelFolderPath();
				path p(levelFolderPath);

				if(is_directory(p)) {
					// iterate over entries in directory
					for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
						std::string singleFile = entry.path().string();

						// we don't want to process directories in the saving directory of the level
						if(vc::isDirectory(singleFile)) continue;

						// create source file
						zip_source_t* source = zip_source_file(archive, singleFile.c_str(), 0, 0);
						if(source == nullptr) {
							throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(archive)));
						}

						// add source file to zip archive
						std::string a = std::string(singleFile).substr(levelFolderPath.length() + 1);
						if(zip_file_add(archive, std::string(singleFile).substr(levelFolderPath.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0) {
							zip_source_free(source);
							throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(archive)));
						}
					}
				}
			} catch(...) {
				zip_close(archive);
				throw;
			}
			zip_close(archive);
		};
	}

	egui::importFuct SelectLevelState::getLambda_importFunc() {
		return [this]() {
			// open file select dialog
			std::optional<std::string> zipArchiveName = openLevelImportDialog();
			if(!zipArchiveName.has_value()) return;

			// open zip archive
			int error = 0;
			zip* archive = zip_open(zipArchiveName.value().c_str(), 0, &error);
			if(archive == nullptr) {
				zip_error_t ziperror;
				zip_error_init_with_code(&ziperror, error);
				throw std::runtime_error("Failed to open output file " + zipArchiveName.value() + ": " + zip_error_strerror(&ziperror));
			}


			// choose name for temporary folder
			int counter = 0;
			const std::string tempFolder_baseName = "tempImport";
			std::string tempFolderName = tempFolder_baseName + std::to_string(counter);

			while(doesExist(WORLD_SAVE_PATH + "\\" + tempFolderName)) {
				counter++;
				tempFolderName = tempFolder_baseName + std::to_string(counter);
			}

			std::string tempFolderPath = WORLD_SAVE_PATH + "\\" + tempFolderName;

			// create temporary import directory
			createDirectory(tempFolderPath);


			char buffer[4096];

			// iterate over all files in zip archive
			for(int counter = 0; counter < zip_get_num_entries(archive, 0); counter++) {
				zip_stat_t sb;
				if(zip_stat_index(archive, counter, 0, &sb) == 0) {
					zip_file_t* zippedFile = zip_fopen_index(archive, counter, 0);
					if(zippedFile == nullptr) {
						throw std::runtime_error("Can't read zipped file!");
					}

					std::ofstream file{ tempFolderPath + "\\" + sb.name, std::ofstream::binary };

					int numberOfBytesReturned;
					while((numberOfBytesReturned = zip_fread(zippedFile, buffer, sizeof(buffer))) > 0) {
						file.write(buffer, numberOfBytesReturned);
					}

					file.close();
					zip_fclose(zippedFile);
				}
			}

			zip_close(archive);

			// check if temporary import directory contains a valid level
			if(!vc::LevelBuilder::doesLevelExist(tempFolderPath)) {
				// if not, delete folder and output error message
				deleteDirectory(tempFolderName);
				throw std::runtime_error("Passed file is not a valid voxelcraft level file!");
			}


			std::string levelName = "";
			// create LevelMetadata for temp folder on stack, since we need it only
			// to retrieve the name of the level
			{
				LevelMetadata tempMetadata(tempFolderPath, LevelBuilder::LEVEL_CONFIG_FILENAME);
				levelName = tempMetadata.getLevelName();
			}

			std::string finalLevelPath = generateLevelFolderName(levelName);

			// rename temp folder
			boost::filesystem::rename(tempFolderPath, WORLD_SAVE_PATH + "\\" + finalLevelPath);

			// update levelMetadataArr
			updateLevelMetadataArr();
		};
	}

	void SelectLevelState::updateLevelMetadataArr() {
		levelMetadataArr.clear();
		path p(WORLD_SAVE_PATH);

		if(is_directory(p)) {
			for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
				std::string levelFolderPath = entry.path().string();
				if(LevelBuilder::doesLevelExist(levelFolderPath)) {
					std::shared_ptr<LevelMetadata> metadata(new LevelMetadata(levelFolderPath, LevelBuilder::LEVEL_CONFIG_FILENAME));
					levelMetadataArr.push_back(metadata);
				}
			}
		}
	}

	std::string SelectLevelState::generateLevelFolderName(std::string levelFolderName) const {
		while(doesExist(WORLD_SAVE_PATH + "\\" + levelFolderName)) {
			levelFolderName.append("_");
		}
		return levelFolderName;
	}

	void SelectLevelState::updateAndRender(float delta) {
		renderer.beginFrame();
		scene.render(renderer);
		renderer.endFrame();
	}
	void SelectLevelState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
	}
}
