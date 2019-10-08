#include "SelectLevelState.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include "../../model/utils/FileUtils.h"
#include "StateManager.h"

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
			[this](std::shared_ptr<LevelMetadata> metadata) {
				Level* p_l = LevelBuilder::loadLevel(metadata, this->vaoCreator, this->threadPool);

				getStateMananger().setCurrentLevel(p_l);
				getStateMananger().changeState("IngameState");
			},
			[this](std::shared_ptr<LevelMetadata> metadata) {
				LevelBuilder::deleteLevel(metadata->getLevelFolderPath());
				updateLevelMetadataArr();
			},
			[this]() {
				int counter = 0;
				std::string fileName = "save0";

				while(doesExist(WORLD_SAVE_PATH + "\\" + fileName)) {
					counter++;
					fileName = "save" + std::to_string(counter);
				}
				Level* p_l = LevelBuilder::createLevel(WORLD_SAVE_PATH + "\\" + fileName, fileName, 100, this->vaoCreator, this->threadPool);
				
				updateLevelMetadataArr();
				getStateMananger().setCurrentLevel(p_l);
				getStateMananger().changeState("IngameState");
			}
		));
		elem->setOnBackBtn({[this](egui::ActionEvent&) {
			getStateMananger().changeState("MainMenuState");
		}});
		this->scene.setRootElement(elem);
	}

	SelectLevelState::~SelectLevelState() {
		getStateMananger().setCurrentLevel(nullptr);
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
