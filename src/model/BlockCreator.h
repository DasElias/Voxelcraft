#pragma once
#include "TextureOrientation.h"
#include "BlockType.h"
#include "Face.h"
#include "Chunk.h"
#include "Block.h"

namespace vc {
	Block* getBlock(int worldX, int worldY, int worldZ, const std::shared_ptr<BlockType>& type, Chunk& chunk, Face placedOn = Face::BOTTOM, std::int8_t metadata = 0);
	Block* getBlock(int worldX, int worldY, int worldZ, const std::shared_ptr<BlockType>& type, const TextureOrientation& texOrientation, Chunk& chunk, std::int8_t metadata = 0);

}
