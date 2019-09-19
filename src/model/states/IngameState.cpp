#include "IngameState.h"

#include <input/IOHandler.h>
#include <iostream>

#include "../../rendering/LevelRenderer.h"
#include "../utils/TimeUtils.h"
#include "StateManager.h"

#include <model/nodes/UnorganizedParentElement.h>
#include <model/nodeComponents/background/TexturedBackground.h>
#include "../utils/FileUtils.h"

namespace vc {
	IngameState::IngameState(StateManager& stateManager, LevelRenderer& levelRenderer, FpsProvider& fpsProvider, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			levelRenderer(levelRenderer),
			renderer2D(),
			fpsProvider(fpsProvider),
			eguiRenderer(eguiRenderer), 
			fpsLabel(new egui::Label("FPS: NaN", 20, false, egui::Text::HorizontalAlignment::LEFT, egui::Text::VerticalAlignment::TOP, {1, 1, 1})),
			autosaveLabel(new egui::Label("", 0.15f, true, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, { 1, 1, 1 })),
			labelScene(std::shared_ptr<egui::Node>(new egui::UnorganizedParentElement({fpsLabel, autosaveLabel}))),

			crosshairImage(getApplicationFolder().append("\\textures\\gui\\crosshair.png")),
			crosshair(new egui::Label()) {

		crosshair->setPreferredDimension(30, false, 30, false);
	}


	void IngameState::updateAndRender(float delta) {
		if(egui::getInputHandler().isKeyDown(KEY_ESCAPE) && getTimeStateWasEntered() + PRESS_ESC_DELAY < getMilliseconds()) {
			stateManager.changeState("PauseState");
		}

		levelRenderer.render(delta);
		stateManager.getCurrentLevel()->update(delta);

		// compute fps avg
		if(fpsArrList.size() == 5) {
			fpsAvg = 0;
			fpsMin = 0;
			fpsMax = 0;

			for(auto& i : fpsArrList) {
				fpsAvg += i;

				if(i > fpsMax) fpsMax = i;
				if(i < fpsMin || fpsMin == 0) fpsMin = i;
			}

			fpsAvg /= fpsArrList.size();
			fpsArrList.clear();
		}

		fpsArrList.push_back(1.0f / delta);

		if(egui::getInputHandler().isKeyDown(KEY_J)) {
			std::cout << (1.0f / delta) << std::endl;
		}

		// render GUI
		eguiRenderer.beginFrame();
		fpsLabel->setText("FPSAvg: " + std::to_string(fpsAvg) + 
						"\nFPSmin: " + std::to_string(fpsMin) + 
						"\nFPSmax: " + std::to_string(fpsMax));
		float secondsUntilAutosave = stateManager.getCurrentLevel()->getSecondsUntilAutosave();
		autosaveLabel->setText((secondsUntilAutosave > 10) ? "" : std::to_string(int(secondsUntilAutosave)));

		labelScene.render(eguiRenderer);
		eguiRenderer.endFrame();
		
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
		glBindTexture(GL_TEXTURE_2D, crosshairImage.getTexId());
		renderer2D.render(crosshair);
		glDisable(GL_BLEND);
		
		
	}

	void IngameState::onStateEnter() {
		State::onStateEnter();
		egui::getInputHandler().setCursorInputMode(CURSOR_DISABLED);
	}

	void IngameState::onCurrentLevelChange(Level* p_oldLevel, Level* p_newLevel) {
		State::onCurrentLevelChange(p_oldLevel, p_newLevel);
		levelRenderer.setCurrentLevel(p_newLevel);
	}

	void IngameState::onStateExit() {
		State::onStateExit();
		egui::getInputHandler().setCursorInputMode(CURSOR_NORMAL);
	}

}
