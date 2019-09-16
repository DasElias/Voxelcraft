#include "Bedrock.h"

namespace vc::model {
	Bedrock::Bedrock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, Chunk& c, const TextureOrientation& texOrientation, int8_t metadata) :
			Block(inChunkX, inChunkY, inChunkZ, BlockType::BEDROCK.get(), c, texOrientation, metadata) {
	}

	void Bedrock::onDestroy(BlockBreakEvent& evt) {
		if(evt.getPlayer() != nullptr) evt.cancel();
	}


}

