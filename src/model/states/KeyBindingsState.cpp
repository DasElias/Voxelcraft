#include "KeyBindingsState.h"
#include "StateManager.h"
namespace vc {
	KeyBindingsState::KeyBindingsState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx) :
			State(stateManager),
			renderer(eguiRenderer),
			elem(new egui::KeyBindingsElement(eguiCtx)),
			scene(elem),
			previousStateName("MainMenuState") {

		elem->setOnBackButtonAction({[this](egui::ActionEvent& e) {
			getStateMananger().changeState(previousStateName);
		}});
	}

	void KeyBindingsState::updateAndRender(float delta) {
		renderer.beginFrame();

		scene.render(renderer);

		renderer.endFrame();
	}

	void KeyBindingsState::onStateEnter() {
		State::onStateEnter();

		this->previousStateName = getStateMananger().getCurrentStateName();
		scene.discardMouseEvents();
	}
}
