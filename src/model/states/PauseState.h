#pragma once

#include <vector>
#include "../../rendering/LevelRenderer.h"
#include "FpsProvider.h"
#include "State.h"

#include <model\scene\Scene.h>
#include "../../eguiImpl/nodes/PauseStateElement.h"


namespace vc {
	namespace model {
		class PauseState : public State {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static int const PRESS_ESC_DELAY = 200;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				vc::rendering::LevelRenderer& levelRenderer;

				egui::MasterRenderer& eguiRenderer;
				std::shared_ptr<egui::PauseStateElement> elem;
				egui::Scene scene;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				PauseState(StateManager& stateManager, vc::rendering::LevelRenderer& levelRenderer, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx);


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void updateAndRender(float delta) override;
				void onStateEnter() override;
				void onCurrentLevelChange(vc::model::Level* oldLevel, vc::model::Level* newLevel) override;
				void onStateExit() override;
		};

	}
}
