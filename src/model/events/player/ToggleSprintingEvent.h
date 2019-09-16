#pragma once
#include "../Cancellable.h"
#include "PlayerEvent.h"

namespace vc {
	namespace model {
		class ToggleSprintingEvent : public PlayerEvent, public Cancellable {

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			public:
				bool const field_isNowSprinting;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ToggleSprintingEvent(AbstractPlayer* const p_player, bool isNowSprinting);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				bool isNowSprinting() const;
		};
	}
}
