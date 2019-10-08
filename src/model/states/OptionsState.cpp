#include "OptionsState.h"

namespace vc {
	OptionsState::OptionsState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx) :
			State(stateManager),
			renderer(eguiRenderer),
			elem(new egui::OptionsStateElement(eguiCtx)),
			scene(elem) {
	}

	void OptionsState::updateAndRender(float delta) {
		renderer.beginFrame();

		scene.render(renderer);

		renderer.endFrame();
	}

	void OptionsState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
	}
}
