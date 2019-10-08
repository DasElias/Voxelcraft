#include "MainMenuState.h"

#include "../LevelBuilder.h"
#include "../utils/FileUtils.h"
#include "IngameState.h"
#include "StateManager.h"


namespace vc {


	MainMenuState::MainMenuState(StateManager& stateManager, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx) :
			State(stateManager),
			vaoCreator(vaoCreator),
			threadPool(threadPool),
			renderer(eguiRenderer),
			elem(new egui::MainMenuElement(eguiCtx)),
			scene(elem) {

		elem->setOnStartGame({[this](egui::ActionEvent&) {
			startGame();
		}});
		elem->setOnOpenOptions({ [this](egui::ActionEvent&) {
			openOptions();
		}});
		elem->setOnExitGame({ [this](egui::ActionEvent&) {
			exitGame();
		}});
	}

	void MainMenuState::updateAndRender(float delta) {
		renderer.beginFrame();

		scene.render(renderer);

		renderer.endFrame();
	}

	void MainMenuState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
	}

	void MainMenuState::startGame() {
		stateManager.changeState("SelectLevelState");
		
	}

	void MainMenuState::openOptions() {
		stateManager.changeState("KeyBindingsState");
	}

	void MainMenuState::exitGame() {
		exit(0);
	}
}
