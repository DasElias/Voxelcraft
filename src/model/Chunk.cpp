#include "Chunk.h"

#include <iostream>
#include <cmath>
#include <cstdint>

#include "../renderModel/AbstractChunkVaoManager.h"
#include "BlockType.h"
#include "events/block/BlockBreakEvent.h"
#include "events/block/BlockPlaceEvent.h"
#include <model/events/EventManager.h>
#include "Face.h"
#include "Frustum.h"
#include "Player.h"
#include "PlayerPosition.h"
#include "TextureOrientation.h"
#include "utils/MathUtils.h"

namespace vc::model {
	float const Chunk::CHUNK_SIZE_FLOAT = CHUNK_SIZE;


	Chunk::Chunk(glm::ivec3 chunkCoords, Level& lvl, ChunkStack& chunkStack) :
		chunkCoords(chunkCoords),
		level(lvl),
		chunkStack(chunkStack),
		p_chunkVaoObject(level.getVaoManager().getNewVao()) {
	}

	Chunk::~Chunk() {
		level.getVaoManager().returnVao(*p_chunkVaoObject);

		for(auto& b : blocks) {
			delete b;
		}
	}

	void Chunk::createBlock(Block* p_block, uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, std::vector<vc::renderingModel::ChunkVaoData>& data) {
		const TextureOrientation& orientation = p_block->getTexOrientation();

		//left
		if ((inChunkX == 0 && chunkStack.getLeftNeighbor() == nullptr) || (inChunkX == 0 && chunkStack.getLeftNeighbor()->getChunk(chunkCoords.y)->getBlock(CHUNK_SIZE - 1, inChunkY, inChunkZ) == nullptr) || (inChunkX > 0 && getBlock(inChunkX - 1, inChunkY, inChunkZ) == nullptr)) {
			addToDataArray(data, p_block, LEFT, orientation.getLeft());
		}
		//right
		if ((inChunkX == (CHUNK_SIZE - 1) && chunkStack.getRightNeighbor() == nullptr) || (inChunkX == (CHUNK_SIZE - 1) && chunkStack.getRightNeighbor()->getChunk(chunkCoords.y)->getBlock(0, inChunkY, inChunkZ) == nullptr) || (inChunkX < (CHUNK_SIZE - 1) && getBlock(inChunkX + 1, inChunkY, inChunkZ) == nullptr)) {
			addToDataArray(data, p_block, RIGHT, orientation.getRight());
		}
		//bottom
		if ((inChunkY == 0 && chunkCoords.y == 0) || (inChunkY == 0 && chunkCoords.y > 0 && chunkStack.getChunk(chunkCoords.y - 1)->getBlock(inChunkX, CHUNK_SIZE - 1, inChunkZ) == nullptr) || (inChunkY > 0 && getBlock(inChunkX, inChunkY - 1, inChunkZ) == nullptr)) {
			addToDataArray(data, p_block, BOTTOM, orientation.getBottom());
		}
		//top
		if ((inChunkY == (CHUNK_SIZE - 1) && chunkCoords.y < (ChunkStack::AMOUNT_OF_CHUNKS - 1) && chunkStack.getChunk(chunkCoords.y + 1)->getBlock(inChunkX, 0, inChunkZ) == nullptr) || (inChunkY < (CHUNK_SIZE - 1) && getBlock(inChunkX, inChunkY + 1, inChunkZ) == nullptr)) {
			addToDataArray(data, p_block, TOP, orientation.getTop());
		}
		//front
		if ((inChunkZ == 0 && chunkStack.getFrontNeighbor() == nullptr) || (inChunkZ == 0 && chunkStack.getFrontNeighbor()->getChunk(chunkCoords.y)->getBlock(inChunkX, inChunkY, CHUNK_SIZE - 1) == nullptr) || (inChunkZ > 0 && getBlock(inChunkX, inChunkY, inChunkZ - 1) == nullptr)) {
			addToDataArray(data, p_block, FRONT, orientation.getFront());
		}
		//back
		if ((inChunkZ == (CHUNK_SIZE - 1) && chunkStack.getBackNeighbor() == nullptr) || (inChunkZ == (CHUNK_SIZE - 1) && chunkStack.getBackNeighbor()->getChunk(chunkCoords.y)->getBlock(inChunkX, inChunkY, 0) == nullptr) || (inChunkZ < (CHUNK_SIZE - 1) && getBlock(inChunkX, inChunkY, inChunkZ + 1) == nullptr)) {
			addToDataArray(data, p_block, BACK, orientation.getBack());
		}
	}


	void Chunk::addToDataArray(std::vector<vc::renderingModel::ChunkVaoData>& data, Block* p_block, std::int8_t facing, std::int8_t tex) {
		data.push_back({
			float(p_block->getWorldX()), float(p_block->getWorldY()), float(p_block->getWorldZ()), 
			short(((p_block->getBlockType().getTextureFiles()[tex])->getGlobalTextureId() << 3) | facing)
		});
	}

	bool Chunk::wasLastTimeVisible() const {
		return lastTimeVisible;
	}

	void Chunk::setLastTimeVisible(bool lastTimeVisible) {
		this->lastTimeVisible = lastTimeVisible;
	}

	float Chunk::getSquaredDistanceToPlayer() const {
		return squaredDistanceToPlayer;
	}

	void Chunk::updateDistanceToPlayer() {
		squaredDistanceToPlayer = computeDistanceToPointSquared(level.getPlayer().getPosition());
	}

	int Chunk::getMaxHeight() const {
		return maxHeight;
	}

	vc::renderingModel::ChunkVertexArrayObject* const Chunk::getVao() const {
		return p_chunkVaoObject;
	}

	int Chunk::getAmountOfPlacedBlocks() const {
		return placedBlockCounter;
	}

	void Chunk::updateVao() {
		if(chunkStack.isAtLeastOneNeighborNull()) {
			//TODO logging
			std::cerr << "ChunkStack is updated, even though at least one neighbour is null.";
		}

		static std::vector<vc::renderingModel::ChunkVaoData> data;
		data.clear();

		//if(this->positionData.capacity() < p_chunkVaoObject->getPosDataLength()) this->positionData.reserve(p_chunkVaoObject->getPosDataLength());

		maxHeight = 0;

		for(int inChunkX = 0; inChunkX < CHUNK_SIZE; inChunkX++) {
			for(int inChunkY = 0; inChunkY < CHUNK_SIZE; inChunkY++) {
				for(int inChunkZ = 0; inChunkZ < CHUNK_SIZE; inChunkZ++) {
					Block* p_b = getBlock(inChunkX, inChunkY, inChunkZ);

					if(p_b == nullptr) continue;
					else {
						if(inChunkY > maxHeight) maxHeight = inChunkY;
					}

					createBlock(p_b, inChunkX, inChunkY, inChunkZ, data);
				}
			}
		}

		p_chunkVaoObject->updateData(data);
	}

	void Chunk::placeBlockWithoutUpdateAndEvent(Block* p_block, uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ) {
		Block* p_replacedBlock = getBlock(inChunkZ, inChunkY, inChunkX);

		if(p_replacedBlock == nullptr && p_block != nullptr) placedBlockCounter++;
		else if (p_replacedBlock != nullptr && p_block == nullptr) placedBlockCounter--;

		blocks[inChunkX + CHUNK_SIZE * inChunkY + CHUNK_SIZE_SQUARED * inChunkZ] = p_block;
		chunkStack.setShouldSaveFlag();
	}

	void Chunk::placeBlock(Block* p_block, uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, Player* p_triggeringPlayer, bool fireEvent, bool shouldUpdateVao) {
		if(fireEvent) {
			// query block which is currently at the replaced position
			Block* p_replacedBlock = getBlock(inChunkX, inChunkY, inChunkZ);
			bool hasBeenCancelled = false;

			/*
			 * if air should replace air, nothing is done and no event is fired
			 * if the block to place is air, a BlockBreakEvent shoudl be fired
			 * else a BlockPlaceEvent should be fired
			 */
			if(p_replacedBlock == nullptr && p_block == nullptr) return;
			else if (p_block == nullptr) {
				BlockBreakEvent breakEvent(p_replacedBlock, p_triggeringPlayer);
				level.getBlockBreakEventManager().fireEvent(breakEvent);
				hasBeenCancelled |= breakEvent.isCancelled();

				if(! hasBeenCancelled) {
					// we know that p_replacedBlock is not nullptr since we have covered this case with the first if-clause
					p_replacedBlock->onDestroy(breakEvent);
				}
				hasBeenCancelled |= breakEvent.isCancelled();
			} else {
				BlockPlaceEvent placeEvent(p_block, p_replacedBlock, p_triggeringPlayer);
				level.getBlockPlaceEventManager().fireEvent(placeEvent);
				hasBeenCancelled |= placeEvent.isCancelled();
			}

			if(hasBeenCancelled) return;
		}

		// place block
		placeBlockWithoutUpdateAndEvent(p_block, inChunkX, inChunkY, inChunkZ);

		if(shouldUpdateVao) {
			updateVao();

			if (inChunkX == 0) {
				chunkStack.getLeftNeighbor()->getChunk(chunkCoords.y)->updateVao();
			} else if (inChunkX == (CHUNK_SIZE - 1)) {
				chunkStack.getRightNeighbor()->getChunk(chunkCoords.y)->updateVao();
			}
			if (inChunkZ == 0) {
				chunkStack.getFrontNeighbor()->getChunk(chunkCoords.y)->updateVao();
			} else if (inChunkZ == (CHUNK_SIZE - 1)) {
				chunkStack.getBackNeighbor()->getChunk(chunkCoords.y)->updateVao();
			}
			if (inChunkY == 0 && chunkCoords.y > 0) {
				chunkStack.getChunk(chunkCoords.y - 1)->updateVao();
			} else if (inChunkY == (CHUNK_SIZE - 1) && chunkCoords.y < (ChunkStack::AMOUNT_OF_CHUNKS - 1)) {
				chunkStack.getChunk(chunkCoords.y + 1)->updateVao();
			}
		}
	}

	float Chunk::computeDistanceToPointSquared(float x, float y, float z) const {
		glm::vec3 directionVector(
			convertChunkToWorldValue(chunkCoords.x, CHUNK_SIZE / 2) - x,
			convertChunkToWorldValue(chunkCoords.y, CHUNK_SIZE / 2) - y,
			convertChunkToWorldValue(chunkCoords.z, CHUNK_SIZE / 2) - z);

		return abs(directionVector.x * directionVector.x + directionVector.y * directionVector.y + directionVector.z * directionVector.z);
	}

	float Chunk::computeDistanceToPointSquared(const glm::vec3& point) const {
		return computeDistanceToPointSquared(point.x, point.y, point.z);
	}

	float Chunk::computeDistanceToPoint(float x, float y, float z) const {
		return sqrtf(computeDistanceToPointSquared(x, y, z));
	}

	float Chunk::computeDistanceToPoint(const glm::vec3& point) const {
		return computeDistanceToPoint(point.x, point.y, point.z);
	}

	bool Chunk::isInFrustum() const {
		if(placedBlockCounter == 0) return false;

		Frustum& f = level.getPlayer().getFrustum();
		return f.isInFrustum_aabb(float(chunkCoords.x * CHUNK_SIZE), float(chunkCoords.y * CHUNK_SIZE), float(chunkCoords.z * CHUNK_SIZE), CHUNK_SIZE_FLOAT, CHUNK_SIZE_FLOAT, CHUNK_SIZE_FLOAT);
	}

	glm::ivec3 Chunk::getChunkCoordinates() const {
		return chunkCoords;
	}

	Block* const Chunk::getBlock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ) {
		return blocks[inChunkX + CHUNK_SIZE * inChunkY + CHUNK_SIZE_SQUARED * inChunkZ];
	}

	Level& Chunk::getLevel() {
		return level;
	}
}
