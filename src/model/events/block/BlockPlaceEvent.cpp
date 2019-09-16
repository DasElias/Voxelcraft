#include "BlockPlaceEvent.h"

namespace vc::model {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	BlockPlaceEvent::BlockPlaceEvent(AbstractBlock* const p_newBlock, AbstractBlock* const p_replacedBlock, AbstractPlayer* const p_player) :
			BlockEvent(p_newBlock, p_player),
			p_replacedBlock(p_replacedBlock) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	AbstractBlock* const BlockPlaceEvent::getReplacedBlock() const {
		return p_replacedBlock;
	}
}

