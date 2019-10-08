#pragma once

#include "State.h"
#include <model\scene\Scene.h>
#include "../../eguiImpl/nodes/MainMenuElement.h"

namespace vc {
	class MainMenuState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			AbstractChunkVaoManager& vaoCreator;
			ctpl::thread_pool& threadPool;
			
			egui::MasterRenderer& renderer;
			std::shared_ptr<egui::MainMenuElement> elem;
			egui::Scene scene;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MainMenuState(StateManager& stateManager, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void startGame();
			void openOptions();
			void exitGame();

		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
	};

}
