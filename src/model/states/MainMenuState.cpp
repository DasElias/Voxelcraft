#include "MainMenuState.h"

#include "../LevelBuilder.h"
#include "../utils/FileUtils.h"
#include "IngameState.h"
#include "StateManager.h"


namespace vc {

	const std::string MainMenuState::WORLD_SAVE_PATH = getApplicationFolder().append("\\save");

	MainMenuState::MainMenuState(StateManager& stateManager, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& eguiCtx) :
			State(stateManager),
			vaoCreator(vaoCreator),
			threadPool(threadPool),
			renderer(eguiRenderer),
			elem(new egui::MainMenuElement(eguiCtx)),
			scene(elem) {

		elem->setOnCreateGameScore({[this](egui::ActionEvent&) {
			createGameScore();
		}});
		elem->setOnLoadGameScore({[this](egui::ActionEvent&) {
			loadGameScore();
		}});
		elem->setOnOpenOptions({ [this](egui::ActionEvent&) {
			openOptions();
		}});
		elem->setOnExitGame({ [this](egui::ActionEvent&) {
			exitGame();
		}});
	}

	MainMenuState::~MainMenuState() {
		unloadLevel();
	}

	void MainMenuState::updateAndRender(float delta) {
		renderer.beginFrame();

		scene.render(renderer);

		renderer.endFrame();
	}

	void MainMenuState::changeStateAndLoadLevel(Level* p_l) {
		unloadLevel();
		stateManager.setCurrentLevel(p_l);
		stateManager.changeState("IngameState");

	}

	void MainMenuState::onStateEnter() {
		elem->setLoadButtonDisabled(! doesLevelExist(WORLD_SAVE_PATH));
	}

	void MainMenuState::createGameScore() {
		if(doesLevelExist(WORLD_SAVE_PATH)) {
			deleteLevel(WORLD_SAVE_PATH);
		}

		Level* p_l = createLevel(WORLD_SAVE_PATH, 100, vaoCreator, threadPool);
		changeStateAndLoadLevel(p_l);
	}

	void MainMenuState::loadGameScore() {
		Level* p_l = loadLevel(WORLD_SAVE_PATH, vaoCreator, threadPool);
		changeStateAndLoadLevel(p_l);
	}

	void MainMenuState::openOptions() {
		stateManager.changeState("OptionsState");
	}

	void MainMenuState::exitGame() {
		exit(0);
	}
	void MainMenuState::unloadLevel() {
		if(stateManager.getCurrentLevel() != nullptr) {
			Level* p_currentLevel = stateManager.getCurrentLevel();
			stateManager.setCurrentLevel(nullptr);
			delete p_currentLevel;
		}
	}
}
