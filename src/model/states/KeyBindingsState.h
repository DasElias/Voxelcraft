#pragma once

#include "State.h"
#include <model\scene\Scene.h>
#include "../../eguiImpl/nodes/KeyBindingsElement.h"

namespace vc {
	class KeyBindingsState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:		
			egui::MasterRenderer& renderer;
			std::shared_ptr<egui::KeyBindingsElement> elem;
			egui::Scene scene;
			std::string previousStateName;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			KeyBindingsState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
	};

}
