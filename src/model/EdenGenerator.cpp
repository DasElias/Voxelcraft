#include "EdenGenerator.h"

#include "Chunk.h"
#include <cmath>

#include "SimplexNoise.h"
#include "BlockCreator.h"
#include "utils/MathUtils.h"

using namespace std;

namespace vc::model {
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

		array<array<int, Chunk::CHUNK_SIZE>, Chunk::CHUNK_SIZE> height;
		array<array<int, Chunk::CHUNK_SIZE>, Chunk::CHUNK_SIZE> bedrockHeight;
		for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
			for(int z = 0; z < Chunk::CHUNK_SIZE; z++) { 
				int worldX = convertChunkToWorldValue(chunkStackCoordinates.x, x);
				int worldZ = convertChunkToWorldValue(chunkStackCoordinates.y, z);

				// we are allowed to do this conversion here since height will never be greater than INT_MAX.
				height[x][z] = int(roundf(20 * (perlinNoise(worldX, worldZ, level.getSeed()) + 2)));
				bedrockHeight[x][z] = int(roundf(2 * (perlinNoise(worldX, worldZ, level.getSeed()) + 2)));

			}
		}

		for(int chunkCoordinateY = 0; chunkCoordinateY < ChunkStack::AMOUNT_OF_CHUNKS; chunkCoordinateY++) {
			Chunk* chunk = new Chunk(glm::ivec3(chunkStackCoordinates.x, chunkCoordinateY, chunkStackCoordinates.y), level, *chunkStack);
			chunkStack->placeChunk(chunkCoordinateY, chunk);

			for(int inChunkX = 0; inChunkX < Chunk::CHUNK_SIZE; inChunkX++) {
				for(int inChunkZ = 0; inChunkZ < Chunk::CHUNK_SIZE; inChunkZ++) {
					int toPlace;
					if(height[inChunkX][inChunkZ] >= Chunk::CHUNK_SIZE) {
						height[inChunkX][inChunkZ] -= Chunk::CHUNK_SIZE;
						toPlace = Chunk::CHUNK_SIZE;
					} else {
						toPlace = height[inChunkX][inChunkZ];
						height[inChunkX][inChunkZ] = 0;
					}

					for(int inChunkY = 0; inChunkY < toPlace; inChunkY++) {
						int worldX = convertChunkToWorldValue(chunkStackCoordinates.x, inChunkX);
						int worldY = convertChunkToWorldValue(chunkCoordinateY, inChunkY);
						int worldZ = convertChunkToWorldValue(chunkStackCoordinates.y, inChunkZ);

						const std::shared_ptr<BlockType>& blockTypeToPlace = (worldY <= bedrockHeight[inChunkX][inChunkZ]) ? BlockType::BEDROCK : BlockType::GRASS;

						Block* block = getBlock(worldX, worldY, worldZ, blockTypeToPlace, *chunk);
						chunk->placeBlockWithoutUpdateAndEvent(block, inChunkX, inChunkY, inChunkZ);
					}
				}
			}
		}

		chunkStack->clearShouldSaveFlag();
		return chunkStack;

	}

}

