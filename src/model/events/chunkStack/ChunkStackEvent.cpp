#include "ChunkStackEvent.h"

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	ChunkStackEvent::ChunkStackEvent(AbstractChunkStack* const p_chunkStack) :
			p_chunkStack(p_chunkStack) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	AbstractChunkStack* const ChunkStackEvent::getChunkStack() const {
		return p_chunkStack;
	}

}

