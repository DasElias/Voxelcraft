#include "BlockEvent.h"
#include "../../Block.h"

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	BlockEvent::BlockEvent(AbstractBlock* const p_block, AbstractPlayer* const p_player) :
			p_block(p_block), p_player(p_player) {
	}


	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	AbstractBlock* const BlockEvent::getBlock() const {
		return p_block;
	}

	AbstractPlayer* const BlockEvent::getPlayer() const {
		return p_player;
	}

}
