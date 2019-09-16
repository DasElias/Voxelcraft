#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "LevelRenderer.h"

#include <input/IOHandler.h>
#include <iostream>
#include <limits>
#include <climits>

#include "../model/BlockType.h"
#include "../model/utils/FileUtils.h"
#include "../model/utils/TimeUtils.h"



namespace vc::rendering {
	namespace {
		vc::renderingModel::TextureArray genTextureArray() {
			// get the folders, where all the textures are stored
			std::string applFolder = vc::model::getApplicationFolder().append("\\textures\\");

			// put all the TextureFile-objects in an array
			std::vector<vc::model::TextureFile> allTextureFiles;
			for(auto& bt : vc::model::BlockType::getAll()) {
				std::array<tl::optional<vc::model::TextureFile>, 6> texFiles = bt.second->getTextureFiles();
				for(auto& texFile : texFiles) {
					if(! texFile.has_value()) continue;

					allTextureFiles.push_back(texFile.value());
				}
			}

			// sort the texture files by texture array
			std::sort(allTextureFiles.begin(), allTextureFiles.end(), 
				[](const vc::model::TextureFile& o1, const vc::model::TextureFile& o2) -> bool {
					// this lambda returns true if the first argument should before the second one
					// we want to sort the list in ascending order
					// => return true if a < b

					return o1.getGlobalTextureId() < o2.getGlobalTextureId();
				}
			);

			// remove duplicates
			auto compareFunction = [](const vc::model::TextureFile& o1, const vc::model::TextureFile& o2) {
				return o1.getGlobalTextureId() == o2.getGlobalTextureId();
			};
			allTextureFiles.erase(unique(allTextureFiles.begin(), allTextureFiles.end(), compareFunction), allTextureFiles.end());

			// generate path of all the textures
			std::vector<std::string> texturePaths;
			for(auto& texFile : allTextureFiles) {
				texturePaths.push_back(applFolder + (texFile.getPath().append(".png")));
			}

			return vc::renderingModel::TextureArray(texturePaths);
		}
	}
	LevelRenderer::LevelRenderer() :
			blockTextureArray(genTextureArray()),
			blockRenderer(blockTextureArray),
			blockInHandRenderer(blockTextureArray),
			multisampleFbo({new vc::renderingModel::FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight(), MULTISAMPLES)}),
			outputFbo({new vc::renderingModel::FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight())}),
			postProcessor(),
			lastFrameWindowWidth(egui::getDisplayHandler().getFramebufferWidth()),
			lastFrameWindowHeight(egui::getDisplayHandler().getFramebufferHeight()),
			lastTimeResizing(LLONG_MAX - RESIZING_FBO_DELAY_MS) {

	}

	LevelRenderer::~LevelRenderer() {
		blockTextureArray.cleanUp();
	}

	bool LevelRenderer::updateFbos() {
		int newWidth = egui::getDisplayHandler().getWidth();
		int newHeight = egui::getDisplayHandler().getHeight();

		if(newWidth != this->lastFrameWindowWidth || newHeight != this->lastFrameWindowHeight) {
			this->lastTimeResizing = vc::model::getMilliseconds();
			this->lastFrameWindowWidth = newWidth;
			this->lastFrameWindowHeight = newHeight;
		}

		if(this->lastTimeResizing + RESIZING_FBO_DELAY_MS < vc::model::getMilliseconds()) {
			this->lastTimeResizing = LLONG_MAX - RESIZING_FBO_DELAY_MS;
			//TODO logging
			return true;
		} else {
			return false;
		}
	}



	void LevelRenderer::setCurrentLevel(Level* p_currentLevel) {
		this->p_currentLevel = p_currentLevel;

		blockInHandRenderer.setCurrentLevel(p_currentLevel);
	}

	void LevelRenderer::render_impl(float delta, bool updateOutputFbo) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		if(updateOutputFbo) {
			multisampleFbo.reset(new vc::renderingModel::FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight(), MULTISAMPLES));
		}

		glm::vec4 backgroundColor = p_currentLevel->getBackgroundColor();


		glActiveTexture(GL_TEXTURE0);
		multisampleFbo->bind();
		multisampleFbo->clear(backgroundColor);


		blockRenderer.render(p_currentLevel->getVisibleChunks(), p_currentLevel);
		//TODO particle renderer
		//TODO crosshair


		blockInHandRenderer.render(p_currentLevel->getPlayer().getBlockTypeInHand());
		multisampleFbo->unbind();

	}

	void LevelRenderer::render(float delta) {
		render_impl(delta, updateFbos());
		multisampleFbo->resolveToScreen();
	}

	void LevelRenderer::renderWithGaussianBlur(float delta) {
		bool updateOutputFbo = updateFbos();

		if(updateOutputFbo) {
			outputFbo.reset(new vc::renderingModel::FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight()));
		}

		render_impl(delta, updateOutputFbo);

		multisampleFbo->resolveToFbo(* outputFbo);
		postProcessor.render(outputFbo->getColorTexture(), updateOutputFbo);
	}

}

