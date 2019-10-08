#include "PauseState.h"

#include <input/IOHandler.h>

#include "../../rendering/LevelRenderer.h"
#include "../utils/TimeUtils.h"
#include "IngameState.h"
#include "StateManager.h"


namespace vc {
	PauseState::PauseState(StateManager& sm, LevelRenderer& levelRenderer, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx) :
			State(sm),
			levelRenderer(levelRenderer),
			eguiRenderer(eguiRenderer),
			elem(new egui::PauseStateElement(eguiCtx)),
			scene(elem) {

		elem->setOnBackToGame({[this](egui::ActionEvent&) {
			stateManager.changeState("IngameState");
		}});
		elem->setOnOptionsBtn({[this](egui::ActionEvent&) {
			stateManager.changeState("KeyBindingsState");
		}});
		elem->setOnMainMenuBtn({[this](egui::ActionEvent&) {
			stateManager.changeState("MainMenuState");
		}});
	}

	void PauseState::updateAndRender(float delta) {
		if(egui::getInputHandler().isKeyDown(KEY_ESCAPE) && getTimeStateWasEntered() + PRESS_ESC_DELAY < getMilliseconds()) {
			stateManager.changeState("IngameState");
		}

		levelRenderer.renderWithGaussianBlur(delta);

		stateManager.getCurrentLevel()->updateOnlyFrustum(delta);

		// render egui
		eguiRenderer.beginFrame();

		scene.render(eguiRenderer);

		eguiRenderer.endFrame();
	}

	void PauseState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
	}

	void PauseState::onCurrentLevelChange(Level* p_oldLevel, Level* p_newLevel) {
		State::onCurrentLevelChange(p_oldLevel, p_newLevel);

		levelRenderer.setCurrentLevel(p_newLevel);
	}

	void PauseState::onStateExit() {
		State::onStateExit();
	}

}
