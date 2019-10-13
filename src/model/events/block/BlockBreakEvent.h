#pragma once

#include "BlockEvent.h"
#include <model/events/Cancellable.h>



namespace vc {
	class BlockBreakEvent : public BlockEvent, public egui::Cancellable {

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			BlockBreakEvent(AbstractBlock* const p_replacedBlock, AbstractPlayer* const p_player);
		};
}
