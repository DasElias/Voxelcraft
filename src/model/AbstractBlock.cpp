#include "AbstractBlock.h"
#include "utils/MathUtils.h"

namespace vc {
	AbstractBlock::AbstractBlock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ) :
			inChunkX(inChunkX),
			inChunkY(inChunkY),
			inChunkZ(inChunkZ) {
	}


	uint8_t AbstractBlock::getInChunkX() const {
		return inChunkX;
	}

	uint8_t AbstractBlock::getInChunkY() const {
		return inChunkY;
	}

	uint8_t AbstractBlock::getInChunkZ() const {
		return inChunkZ;
	}



}
