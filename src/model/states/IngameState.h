#pragma once

#include <vector>
#include "../../rendering/LevelRenderer.h"
#include "FpsProvider.h"
#include "State.h"


namespace vc {
	namespace model {
		class IngameState : public State {
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

				// TODO EGUI

				FpsProvider& fpsProvider;
				std::vector<float> fpsArrList = std::vector<float>();
				float fps = 0;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				IngameState(StateManager& stateManager, vc::rendering::LevelRenderer& levelRenderer, FpsProvider& fpsProvider);


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void updateAndRender(float delta) override;
				void onStateEnter() override;
				void onCurrentLevelChange(vc::model::Level* p_oldLevel, vc::model::Level* p_newLevel) override;
				void onStateExit() override;
		};

	}
}
