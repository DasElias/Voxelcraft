#pragma once
#include "ChunkStackEvent.h"
#include <model/events/Cancellable.h>

namespace vc {
	class ChunkStackUnloadEvent : public ChunkStackEvent, public egui::Cancellable {

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ChunkStackUnloadEvent(AbstractChunkStack* const p_chunkStack);
	};
}
