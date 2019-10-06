#pragma once

#include "Block.h"

namespace vc {
	class Bedrock : public Block {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Bedrock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, Chunk& c, const TextureOrientation& texOrientation);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void onDestroy(BlockBreakEvent& evt) override;
	};
}
