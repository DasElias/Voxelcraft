#include "ChunkSaver.h"

#include <gzstream.h>
#include <tl/optional.hpp>
#include <cstdint>
#include <iostream>
#include <iterator>

#include "Block.h"
#include "BlockCreator.h"
#include "BlockType.h"
#include "Chunk.h"
#include "ChunkStack.h"
#include "Level.h"
#include "TextureOrientation.h"
#include "utils/MathUtils.h"

namespace vc::model {

	void ChunkSaver::lexer(std::string data, LexerResult& result_lexerResult) {
		std::string labelTarget = "";
		std::string contentTarget = "";
		std::string* currentTarget = &labelTarget;

		bool appendToArray = false;
		std::vector<int> arrData;
		bool arrData_hasValue = false;

		for(char& c : data) {
			if(c == ']') {
				appendToArray = false;
			} else if(appendToArray) {
				if(c == ',') {
					if(contentTarget.length() == 0) {
						arrData.push_back(-1);
					} else {
						arrData.push_back(std::stoi(contentTarget));
						contentTarget.clear();
					}
				} else {
					contentTarget.append(std::string(1, c));
				}
			} else if(c == '[') {
				appendToArray = true;

				arrData_hasValue = true;
				arrData.clear();
			} else if(c == ';') {
				if(! arrData_hasValue) {
					result_lexerResult.stringProperties[labelTarget] = contentTarget;
				} else {
					result_lexerResult.arrayProperties[labelTarget] = arrData;

					arrData_hasValue = false;
				}

				labelTarget.clear();
				contentTarget.clear();

				currentTarget = &labelTarget;
			} else if (c == ':') {
				currentTarget = &contentTarget;
			} else {
				currentTarget->append(std::string(1, c));
			}
		}
	}

	ChunkStack* ChunkSaver::parser(LexerResult& lexedGroup, Level& lvl) {
		glm::ivec2 chunkStackCoordinates(std::stoi(lexedGroup.stringProperties.at("xPos")), std::stoi(lexedGroup.stringProperties.at("zPos")));
		ChunkStack* p_createdChunkStack = new ChunkStack(chunkStackCoordinates, lvl);

		for(int chunkCoordY = 0; chunkCoordY < ChunkStack::AMOUNT_OF_CHUNKS; chunkCoordY++) {
			// create every single chunk and add it to the chunk stack
			glm::ivec3 singleChunkCoordinates(chunkStackCoordinates.x, chunkCoordY, chunkStackCoordinates.y);
			Chunk* p_singleChunk = new Chunk(singleChunkCoordinates, lvl, *p_createdChunkStack);
			p_createdChunkStack->placeChunk(chunkCoordY, p_singleChunk);

			std::string propertyName = "data";
			propertyName.append(std::to_string(chunkCoordY));
			std::vector<int> data = lexedGroup.arrayProperties.at(propertyName);

			uint8_t inChunkX = 0;
			uint8_t inChunkY = 0;
			uint8_t inChunkZ = 0;

			unsigned int counter = 0;
			while(counter < data.size()) {
				int blockId = data[counter++];

				if(blockId != -1) {
					int texValue = data[counter++];
					int8_t metadata = data[counter++];

					int worldX = convertChunkToWorldValue(chunkStackCoordinates.x, inChunkX);
					int worldY = convertChunkToWorldValue(chunkCoordY, inChunkY);
					int worldZ = convertChunkToWorldValue(chunkStackCoordinates.y, inChunkZ);

					const std::shared_ptr<BlockType>& blockType = BlockType::getBlockTypeById(blockId);
					const TextureOrientation& texOrientation = TextureOrientation::getStoredTextureOrientation(texValue);
					Block* p_createdBlock = getBlock(worldX, worldY, worldZ, blockType, texOrientation, *p_singleChunk, metadata);
					p_singleChunk->placeBlockWithoutUpdateAndEvent(p_createdBlock, inChunkX, inChunkY, inChunkZ);
				}

				inChunkZ++;
				if(inChunkZ >= Chunk::CHUNK_SIZE) {
					inChunkZ = 0;
					inChunkY++;
				}
				if(inChunkY >= Chunk::CHUNK_SIZE) {
					inChunkY = 0;
					inChunkX++;
				}
			}
		}

		p_createdChunkStack->clearShouldSaveFlag();
		return p_createdChunkStack;
	}

	void ChunkSaver::saveChunkStack(ChunkStack& c, std::string fileName) {
		ogzstream str(fileName.c_str());

		if(! str) throw std::runtime_error("Couldn't save chunk stack!");

		str << "xPos:" << c.getChunkStackPosition().x << ";";
		str << "zPos:" << c.getChunkStackPosition().y << ";";

		for(int counter = 0; counter < ChunkStack::AMOUNT_OF_CHUNKS; counter++) {
			str << "data" << counter << ":[";

			for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
				for(int y = 0; y < Chunk::CHUNK_SIZE; y++) {
					for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
						Block* p_b = c.getChunk(counter)->getBlock(x, y, z);

						if(p_b == nullptr) str << ",";
						else {
							str << p_b->getBlockType().getId() << ",";
							str << p_b->getTexOrientation().getValue() << ",";
							str << (+p_b->getMetadata()) << ",";
						}
					}
				}
			}
			str << "];";
		}

		str.close();
	}

	ChunkStack* ChunkSaver::readChunkStack(std::string fileName, Level& lvl) {
		igzstream str(fileName.c_str());
		std::string data(std::istreambuf_iterator<char>(str), {});
		LexerResult lexerResult;

		lexer(data, lexerResult);
		return parser(lexerResult, lvl);
	}

}
