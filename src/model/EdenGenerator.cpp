#include "EdenGenerator.h"

#include "Chunk.h"
#include <cmath>

#include "SimplexNoise.h"
#include "BlockCreator.h"
#include "utils/MathUtils.h"

using namespace std;

namespace vc {
	namespace {
		float pow2(float exponent) {
			return powf(2, exponent);
		}

		float perlinNoise(int worldX, int worldZ, float seed) {
			float const n = 7.5f;

			// we can do the conversion from float to float here since this calculation doesn't need the accuracy of float
			return float( 0.5 * SimplexNoise::noise(1/pow2(n) * worldX, 1/pow2(n) * worldZ, seed) +
			 			0.25 * SimplexNoise::noise(1/pow2(n - 1) * worldX, 1/pow2(n - 1) * worldZ, seed) +
			  		   0.125 * SimplexNoise::noise(1/pow2(n - 2) * worldX, 1/pow2(n - 2) * worldZ, seed) +
					  0.0625 * SimplexNoise::noise(1/pow2(n - 3) * worldX, 1/pow2(n - 3) * worldZ, seed));
		}
	}
	ChunkStack* EdenGenerator::generateChunkStack(Level& level, glm::ivec2 chunkStackCoordinates) {
		ChunkStack* chunkStack = new ChunkStack(chunkStackCoordinates, level);

		// create Chunk objects
		for(int chunkY = 0; chunkY < ChunkStack::AMOUNT_OF_CHUNKS; chunkY++) {
			Chunk* chunk = new Chunk(glm::ivec3(chunkStackCoordinates.x, chunkY, chunkStackCoordinates.y), level, *chunkStack);
			chunkStack->placeChunk(chunkY, chunk);
		}

		for(int inChunkX = 0; inChunkX < Chunk::CHUNK_SIZE; inChunkX++) {
			int worldX = convertChunkToWorldValue(chunkStackCoordinates.x, inChunkX);

			for(int inChunkZ = 0; inChunkZ < Chunk::CHUNK_SIZE; inChunkZ++) {
				int worldZ = convertChunkToWorldValue(chunkStackCoordinates.y, inChunkZ);
				float const perlin = (perlinNoise(worldX, worldZ, level.getSeed()) + 2);
				
				int terrainHeight = int(roundf(20 * perlin));
				int bedrockHeight = int(roundf(2 * perlin));

				for(int chunkY = 0; chunkY < ChunkStack::AMOUNT_OF_CHUNKS; chunkY++) {
					int toPlace;
					if(terrainHeight >= Chunk::CHUNK_SIZE) {
						terrainHeight -= Chunk::CHUNK_SIZE;
						toPlace = Chunk::CHUNK_SIZE;
					} else {
						toPlace = terrainHeight;
						terrainHeight = 0;
					}

					Chunk* p_chunk = chunkStack->getChunk(chunkY);

					for(int inChunkY = 0; inChunkY < toPlace; inChunkY++) {
						int worldY = convertChunkToWorldValue(chunkY, inChunkY);

						const std::shared_ptr<BlockType>& blockTypeToPlace = (worldY <= bedrockHeight) ? BlockType::BEDROCK : BlockType::GRASS;
						BlockType* blockTypePtr = blockTypeToPlace.get();
						
						Block* block = createBlock_inChunk(inChunkX, inChunkY, inChunkZ, blockTypeToPlace, *p_chunk);
						p_chunk->placeBlockWithoutUpdateAndEvent(block, inChunkX, inChunkY, inChunkZ);
					}
				}
			}
		}


		chunkStack->clearShouldSaveFlag();
		return chunkStack;

	}

}

