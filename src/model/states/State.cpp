#include "State.h"

#include "../utils/TimeUtils.h"
namespace vc {

	State::State(StateManager& stateManager) :
			stateManager(stateManager) {
	}

	StateManager& State::getStateMananger() {
		return stateManager;
	}

	void State::resetTimeStateWasEntered() {
		this->timeStateWasEntered = getMilliseconds();
	}

	void State::onStateEnter() {
		this->timeStateWasEntered = getMilliseconds();
	}

	void State::onStateExit() {
		// empty method body
	}

	void State::onCurrentLevelChange(Level* oldLevel, Level* newLevel) {
		// empty method body
	}

	long long State::getTimeStateWasEntered() const {
		return timeStateWasEntered;
	}

}
