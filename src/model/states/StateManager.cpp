#include "StateManager.h"

namespace vc::model {
	StateManager::StateManager() {
	}

	void StateManager::addState(std::string stateType, State* state) {
		if(state == nullptr) throw std::invalid_argument("state mustn't be nullptr");

		if(allStates.find(stateType) == allStates.end()) {
			// key doesn't exist yet
			allStates.insert(std::make_pair(stateType, state));
		} else {
			throw std::logic_error("This state has already been added!");
		}
	}

	void StateManager::changeState(std::string stateType) {
		if(allStates.find(stateType) == allStates.end()) {
			// key doesn't exist
			throw std::logic_error("This state doesn't exist!");
		} else {
			if(p_currentState != nullptr) p_currentState->onStateExit();
			p_currentState = allStates.at(stateType);
			p_currentState->onStateEnter();
		}
	}

	void StateManager::updateAndRender(float delta) {
		if(p_currentState == nullptr) throw std::logic_error("state hasn't been set yet!");
		p_currentState->updateAndRender(delta);
	}

	void StateManager::setCurrentLevel(Level* level) {
		for(auto& pair : allStates) {
			pair.second->onCurrentLevelChange(p_currentLevel, level);
		}

		this->p_currentLevel = level;
	}

	Level* StateManager::getCurrentLevel() {
		return p_currentLevel;
	}

}

