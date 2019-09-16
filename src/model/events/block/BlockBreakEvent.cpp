#include "BlockBreakEvent.h"

namespace vc::model {
	BlockBreakEvent::BlockBreakEvent(AbstractBlock* const p_replacedBlock, AbstractPlayer* const p_player) :
			BlockEvent(p_replacedBlock, p_player) {

	}
}


