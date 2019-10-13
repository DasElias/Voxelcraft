#pragma once

#include "BlockEvent.h"
#include <model/events/Cancellable.h>


namespace vc {
	class BlockPlaceEvent : public BlockEvent, public egui::Cancellable {

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			AbstractBlock* const p_replacedBlock;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			BlockPlaceEvent(AbstractBlock* const p_newBlock, AbstractBlock* const p_replacedBlock, AbstractPlayer* const p_player);


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			AbstractBlock* const getReplacedBlock() const;
	};
}
