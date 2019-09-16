#pragma once
#include "../../AbstractChunkStack.h"

namespace vc {
	namespace model {
		class ChunkStackEvent {

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			protected:
				AbstractChunkStack* const p_chunkStack;


			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			protected:
				ChunkStackEvent(AbstractChunkStack* const p_chunkStack);

			public:
				ChunkStackEvent(const ChunkStackEvent&) = delete;


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				AbstractChunkStack* const getChunkStack() const;
		};
	}
}
