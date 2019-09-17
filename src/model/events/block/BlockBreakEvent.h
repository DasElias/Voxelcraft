#pragma once

#include "BlockEvent.h"
#include "../Cancellable.h"


namespace vc {
	namespace model {
		class BlockBreakEvent : public BlockEvent, public Cancellable {

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				BlockBreakEvent(AbstractBlock* const p_replacedBlock, AbstractPlayer* const p_player);
		};
	}
}