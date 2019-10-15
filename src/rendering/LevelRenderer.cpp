#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "LevelRenderer.h"

#include <input/IOHandler.h>
#include <limits>
#include <climits>

#include "../model/BlockType.h"
#include "../model/utils/FileUtils.h"
#include "../model/utils/TimeUtils.h"
#include <glm\ext\matrix_transform.hpp>

#include <model/positioning/CenterAllInParentWrapper.h>


namespace vc {
	namespace {
		TextureArray genTextureArray() {
			// get the folders, where all the textures are stored
			std::string applFolder = getApplicationFolder().append("\\textures\\");

			// put all the TextureFile-objects in an array
			std::vector<TextureFile> allTextureFiles;
			for(auto& bt : BlockType::getAll()) {
				std::array<tl::optional<TextureFile>, 6> texFiles = bt.second->getTextureFiles();
				for(auto& texFile : texFiles) {
					if(! texFile.has_value()) continue;

					allTextureFiles.push_back(texFile.value());
				}
			}

			// sort the texture files by texture array
			std::sort(allTextureFiles.begin(), allTextureFiles.end(), 
				[](const TextureFile& o1, const TextureFile& o2) -> bool {
					// this lambda returns true if the first argument should before the second one
					// we want to sort the list in ascending order
					// => return true if a < b

					return o1.getGlobalTextureId() < o2.getGlobalTextureId();
				}
			);

			// remove duplicates
			auto compareFunction = [](const TextureFile& o1, const TextureFile& o2) {
				return o1.getGlobalTextureId() == o2.getGlobalTextureId();
			};
			allTextureFiles.erase(unique(allTextureFiles.begin(), allTextureFiles.end(), compareFunction), allTextureFiles.end());

			// generate path of all the textures
			std::vector<std::string> texturePaths;
			for(auto& texFile : allTextureFiles) {
				texturePaths.push_back(applFolder + (texFile.getPath().append(".png")));
			}

			return TextureArray(texturePaths, LevelRenderer::BLOCK_TEXTURE_DIMENSIONS, LevelRenderer::BLOCK_TEXTURE_DIMENSIONS);
		}
	}
	LevelRenderer::LevelRenderer(egui::MasterRenderer& eguiRenderer, egui::EGuiContext& ctx) :
			blockTextureArray(genTextureArray()),
			blockRenderer(blockTextureArray),
			itemInHandRenderer(blockTextureArray),
			renderer2D(false),
			renderer2D_discardTransparentFragments(true),
			hotbarRenderer(blockTextureArray, renderer2D, eguiRenderer, ctx),
			eguiRenderer(eguiRenderer),
			multisampleFbo({new FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight(), MULTISAMPLES)}),
			outputFbo({new FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight())}),
			postProcessor(),
			lastFrameWindowWidth(egui::getDisplayHandler().getFramebufferWidth()),
			lastFrameWindowHeight(egui::getDisplayHandler().getFramebufferHeight()),
			lastTimeResizing(LLONG_MAX - RESIZING_FBO_DELAY_MS),
			crosshairImage(getApplicationFolder().append("\\textures\\gui\\crosshair.png")),
			crosshair(new egui::Label()) {
			
		crosshair->setOwnPositioning(std::shared_ptr<egui::Positioning>(new egui::CenterAllInParentWrapper()));
		crosshair->setPreferredDimension(30, egui::RelativityMode::ABSOLUTE_VALUE, 30, egui::RelativityMode::ABSOLUTE_VALUE);

	}

	LevelRenderer::~LevelRenderer() {
		blockTextureArray.cleanUp();
		crosshairImage.cleanUp();
	}

	bool LevelRenderer::updateFbos() {
		int newWidth = egui::getDisplayHandler().getWidth();
		int newHeight = egui::getDisplayHandler().getHeight();

		if(newWidth != this->lastFrameWindowWidth || newHeight != this->lastFrameWindowHeight) {
			this->lastTimeResizing = getMilliseconds();
			this->lastFrameWindowWidth = newWidth;
			this->lastFrameWindowHeight = newHeight;
		}

		if(this->lastTimeResizing + RESIZING_FBO_DELAY_MS < getMilliseconds()) {
			this->lastTimeResizing = LLONG_MAX - RESIZING_FBO_DELAY_MS;
			//TODO logging
			return true;
		} else {
			return false;
		}
	}



	void LevelRenderer::setCurrentLevel(Level* p_currentLevel) {
		this->p_currentLevel = p_currentLevel;

		itemInHandRenderer.setCurrentLevel(p_currentLevel);
	}

	void LevelRenderer::render_impl(float delta, bool updateOutputFbo, FrameBufferObject* resolveTo) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// if the window's dimension has changed, we need to create a new FrameBufferObject
		if(updateOutputFbo) {
			multisampleFbo.reset(new FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight(), MULTISAMPLES));
		}

		glm::vec4 backgroundColor = p_currentLevel->getBackgroundColor();
		Player& player = p_currentLevel->getPlayer();

		// bind and clear FBO
		glActiveTexture(GL_TEXTURE0);
		multisampleFbo->bind();
		multisampleFbo->clear(backgroundColor);

		// render chunks
		blockRenderer.render(p_currentLevel->getVisibleChunks(), p_currentLevel);

		// render block in hand
		itemInHandRenderer.render(player.getItemTypeInHand());	

		// render hotbar
		hotbarRenderer.renderHotbar(player.getInventory(), player.getActiveHotbarIndex());
		multisampleFbo->unbind();

		if(resolveTo == nullptr) {
			multisampleFbo->resolveToScreen();
		} else {
			multisampleFbo->resolveToFbo(* resolveTo);
		}

		// we don't need the multisampled FBO to render the inventory
		if(player.isInventoryGUIActive()) {
			player.getInventoryGUI()->render(player, eguiRenderer, blockTextureArray);
		} else {
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			glBindTexture(GL_TEXTURE_2D, crosshairImage.getTexId());
			renderer2D_discardTransparentFragments.render(crosshair);
			glDisable(GL_BLEND);
		}
	}

	void LevelRenderer::render(float delta) {
		render_impl(delta, updateFbos(), nullptr);
	}

	void LevelRenderer::renderWithGaussianBlur(float delta) {
		bool updateOutputFbo = updateFbos();

		if(updateOutputFbo) {
			outputFbo.reset(new FrameBufferObject(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight()));
		}

		render_impl(delta, updateOutputFbo, outputFbo.get());

		postProcessor.render(outputFbo->getColorTexture(), updateOutputFbo);
	}

}

