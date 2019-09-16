#pragma once
#include "../Level.h"


namespace vc {
	namespace model {
		class StateManager;

		class State {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				long long timeStateWasEntered = 0;

			protected:
				StateManager& stateManager;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				State(StateManager& stateManager);
				State(const State&) = delete;
				virtual ~State() = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			protected:
				StateManager& getStateMananger();

			public:
				virtual void updateAndRender(float delta) =0;
				virtual void onStateEnter();
				virtual void onStateExit();
				virtual void onCurrentLevelChange(Level* p_oldLevel, Level* p_newLevel);

				long long getTimeStateWasEntered() const;

		};
	}
}
