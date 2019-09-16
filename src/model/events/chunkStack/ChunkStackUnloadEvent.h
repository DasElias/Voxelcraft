#pragma once
#include "ChunkStackEvent.h"
#include "../Cancellable.h"

namespace vc {
	namespace model {
		class ChunkStackUnloadEvent : public ChunkStackEvent, public Cancellable {

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ChunkStackUnloadEvent(AbstractChunkStack* const p_chunkStack);
		};
	}
}
