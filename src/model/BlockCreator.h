#pragma once
#include "TextureOrientation.h"
#include "BlockType.h"
#include "Face.h"
#include "Chunk.h"
#include "Block.h"

namespace vc {
	Block* createBlock_inChunk(const int& inChunkX, const int& inChunkY, const int& inChunkZ, const std::shared_ptr<BlockType>& type, Chunk& chunk, Face placedOn = Face::BOTTOM, std::string metadata = "");
	Block* createBlock(const int& worldX, const int& worldY, const int& worldZ, const std::shared_ptr<BlockType>& type, Chunk& chunk, Face placedOn = Face::BOTTOM, std::string metadata = "");
	Block* createBlock(const int& worldX, const int& worldY, const int& worldZ, const std::shared_ptr<BlockType>& type, const TextureOrientation& texOrientation, Chunk& chunk, std::string metadata = "");

}
