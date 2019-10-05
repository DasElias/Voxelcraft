#include "BlockCreator.h"

#include <cstdint>

#include "utils/MathUtils.h"

namespace vc {
	Block* createBlock_inChunk(const int& inChunkX, const int& inChunkY, const int& inChunkZ, const std::shared_ptr<BlockType>& type, Chunk& chunk, Face placedOn, std::int8_t metadata) {
		const TextureOrientation& texOrientation = type->getTextureOrientation(placedOn);
		return type->instantiate(inChunkX, inChunkY, inChunkZ, chunk, texOrientation, metadata);

	}
	Block* createBlock(const int& worldX, const int& worldY, const int& worldZ, const std::shared_ptr<BlockType>& type, Chunk& chunk, Face placedOn, std::int8_t metadata) {
		std::uint8_t inChunkX = convertWorldToInChunkValue(float(worldX));
		std::uint8_t inChunkY = convertWorldToInChunkValue(float(worldY));
		std::uint8_t inChunkZ = convertWorldToInChunkValue(float(worldZ));
		return createBlock_inChunk(inChunkX, inChunkY, inChunkZ, type, chunk, placedOn, metadata);
	}

	Block* createBlock(const int& worldX, const int& worldY, const int& worldZ, const std::shared_ptr<BlockType>& type, const TextureOrientation& texOrientation, Chunk& chunk, std::int8_t metadata) {
		std::uint8_t inChunkX = convertWorldToInChunkValue(float(worldX));
		std::uint8_t inChunkY = convertWorldToInChunkValue(float(worldY));
		std::uint8_t inChunkZ = convertWorldToInChunkValue(float(worldZ));

		return type->instantiate(inChunkX, inChunkY, inChunkZ, chunk, texOrientation, metadata);
	}

}

