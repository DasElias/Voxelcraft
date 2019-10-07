#pragma once
#include <typeinfo>
#include <unordered_map>
#include <memory>
#include "State.h"
#include "../Level.h"

namespace vc {
	class StateManager {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::unordered_map<std::string, State*> allStates;
			Level* p_currentLevel = nullptr;
			State* p_currentState = nullptr;
			std::string currentStateName = "";

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			StateManager();
			StateManager(const StateManager&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getCurrentStateName() const;

			void addState(std::string stateType, State* state);
			void changeState(std::string stateType);
			void updateAndRender(float delta);
			void setCurrentLevel(Level* level);
			Level* getCurrentLevel();
	};
}
