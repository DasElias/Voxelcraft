#include "BlockCreator.h"

#include <cstdint>

#include "utils/MathUtils.h"

namespace vc {
	Block* getBlock(int worldX, int worldY, int worldZ, const std::shared_ptr<BlockType>& type, Chunk& chunk, Face placedOn, std::int8_t metadata) {
		const TextureOrientation& texOrientation = type->getTextureOrientation(placedOn);
		return getBlock(worldX, worldY, worldZ, type, texOrientation, chunk, metadata);

	}

	Block* getBlock(int worldX, int worldY, int worldZ, const std::shared_ptr<BlockType>& type, const TextureOrientation& texOrientation, Chunk& chunk, std::int8_t metadata) {
		std::uint8_t inChunkX = convertWorldToInChunkValue(float(worldX));
		std::uint8_t inChunkY = convertWorldToInChunkValue(float(worldY));
		std::uint8_t inChunkZ = convertWorldToInChunkValue(float(worldZ));

		return type->instantiate(inChunkX, inChunkY, inChunkZ, chunk, texOrientation, metadata);
	}

}

