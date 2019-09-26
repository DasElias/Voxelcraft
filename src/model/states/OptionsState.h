#pragma once

#include "State.h"
#include <model\scene\Scene.h>
#include "../../eguiImpl/nodes/OptionsStateElement.h"

namespace vc {
	class OptionsState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:		
			egui::MasterRenderer& renderer;
			std::shared_ptr<egui::OptionsStateElement> elem;
			egui::Scene scene;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OptionsState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
	};

}
