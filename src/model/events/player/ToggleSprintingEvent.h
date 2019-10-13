#pragma once
#include <model/events/Cancellable.h>

#include "PlayerEvent.h"

namespace vc {
	class ToggleSprintingEvent : public PlayerEvent, public egui::Cancellable {

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
