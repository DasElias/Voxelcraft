#include "ToggleSprintingEvent.h"

namespace vc::model {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	ToggleSprintingEvent::ToggleSprintingEvent(AbstractPlayer* const p_player, bool isNowSprinting) :
			PlayerEvent(p_player),
			field_isNowSprinting(isNowSprinting) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	bool ToggleSprintingEvent::isNowSprinting() const {
		return field_isNowSprinting;
	}
}


