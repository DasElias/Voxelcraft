#pragma once

#include <vector>
#include "../../rendering/LevelRenderer.h"
#include "State.h"

#include <model/scene/Scene.h>
#include <model/nodes/Label.h>
#include "../../rendering/Renderer2D.h"
#include "..//..//renderModel/SimpleTexture.h"

namespace vc {
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
			LevelRenderer& levelRenderer;

			egui::MasterRenderer& eguiRenderer;
			std::shared_ptr<egui::Label> fpsLabel;
			std::shared_ptr<egui::Label> autosaveLabel;
			egui::Scene labelScene;



			std::vector<float> fpsArrList = std::vector<float>();
			float fpsAvg = 0, fpsMin = 0, fpsMax = 0;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IngameState(StateManager& stateManager, LevelRenderer& levelRenderer, egui::MasterRenderer& eguiRenderer);


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
			void onCurrentLevelChange(Level* p_oldLevel, Level* p_newLevel) override;
			void onStateExit() override;
	};

}
