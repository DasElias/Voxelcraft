#pragma once

#include "State.h"
#include <model\scene\Scene.h>
#include "../../eguiImpl/nodes/MainMenuElement.h"

namespace vc {
	namespace model {
		class MainMenuState : public State {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static std::string const WORLD_SAVE_PATH;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				vc::renderingModel::AbstractChunkVaoManager& vaoCreator;
				ctpl::thread_pool& threadPool;
				
				egui::MasterRenderer& renderer;
				std::shared_ptr<egui::MainMenuElement> elem;
				egui::Scene scene;


			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				MainMenuState(StateManager& stateManager, vc::renderingModel::AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx);
				~MainMenuState();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				void changeStateAndLoadLevel(Level* p_l);

				void createGameScore();
				void loadGameScore();
				void openOptions();
				void exitGame();

				void unloadLevel();

			public:
				void updateAndRender(float delta) override;
				void onStateEnter() override;
		};

	}
}
