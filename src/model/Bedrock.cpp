#include "Bedrock.h"

namespace vc {
	Bedrock::Bedrock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, Chunk& c, const TextureOrientation& texOrientation) :
			Block(inChunkX, inChunkY, inChunkZ, BlockType::BEDROCK.get(), c, texOrientation) {
	}

	void Bedrock::onDestroy(BlockBreakEvent& evt) {
		if(evt.getPlayer() != nullptr) evt.cancel();
	}


}

