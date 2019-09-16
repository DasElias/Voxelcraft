#include "IngameState.h"

#include <input/IOHandler.h>
#include <iostream>

#include "../../rendering/LevelRenderer.h"
#include "../utils/TimeUtils.h"
#include "StateManager.h"
namespace vc::model {
	IngameState::IngameState(StateManager& stateManager, vc::rendering::LevelRenderer& levelRenderer, FpsProvider& fpsProvider) :
			State(stateManager),
			levelRenderer(levelRenderer),
			fpsProvider(fpsProvider) {
	}


	void IngameState::updateAndRender(float delta) {
		if(egui::getInputHandler().isKeyDown(KEY_ESCAPE) && getTimeStateWasEntered() + PRESS_ESC_DELAY < getMilliseconds()) {
			stateManager.changeState("PauseState");
		}

		levelRenderer.render(delta);
		stateManager.getCurrentLevel()->update(delta);

		// TODO gui rendering
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
