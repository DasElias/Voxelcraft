#include "ChunkStack.h"

#include <array>
#include <cstdint>

#include "Chunk.h"
#include "ChunkIO.h"
#include "utils/MathUtils.h"

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	ChunkStack::ChunkStack(glm::ivec2 chunkStackCoords, Level& level) :
			chunkStackCoords(chunkStackCoords),
			level(level) {
	}

	ChunkStack::~ChunkStack() {
		possiblySavePersistant();

		NeighborAccessKey key;
		if(p_leftNeighbor) p_leftNeighbor->setRightNeighbor(key, nullptr);
		if(p_rightNeighbor) p_rightNeighbor->setLeftNeighbor(key, nullptr);
		if(p_frontNeighbor) p_frontNeighbor->setBackNeighbor(key, nullptr);
		if(p_backNeighbor) p_backNeighbor->setFrontNeighbor(key, nullptr);

		for(auto& chunk : chunkData) {
			delete chunk;
		}
	}

	void ChunkStack::setNeighbour() {
		if(p_leftNeighbor != nullptr && p_rightNeighbor != nullptr && p_frontNeighbor != nullptr && p_backNeighbor != nullptr) {
			if(! isFirstInitialized) {
				level.addChunkStackToLoad({}, *this);
			}
		}

		updateDisabledFlag();
	}

	void ChunkStack::updateDisabledFlag() {
		field_isDisabled = !isFirstInitialized || isAtLeastOneNeighborNull();
	}

	void ChunkStack::setFirstInitialized(NeighborAccessKey key) {
		isFirstInitialized = true;
		updateDisabledFlag();
	}

	void ChunkStack::setLeftNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor) {
		p_leftNeighbor = p_neighbor;
		setNeighbour();
	}

	void ChunkStack::setRightNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor) {
		p_rightNeighbor = p_neighbor;
		setNeighbour();
	}

	void ChunkStack::setFrontNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor) {
		p_frontNeighbor = p_neighbor;
		setNeighbour();
	}

	void ChunkStack::setBackNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor) {
		p_backNeighbor = p_neighbor;
		setNeighbour();
	}

	ChunkStack* const ChunkStack::getLeftNeighbor() const {
		return p_leftNeighbor;
	}

	ChunkStack* const ChunkStack::getRightNeighbor() const {
		return p_rightNeighbor;
	}

	ChunkStack* const ChunkStack::getFrontNeighbor() const {
		return p_frontNeighbor;
	}

	ChunkStack* const ChunkStack::getBackNeighbor() const {
		return p_backNeighbor;
	}

	bool ChunkStack::isAtLeastOneNeighborNull() const {
		return !p_leftNeighbor || !p_rightNeighbor || !p_frontNeighbor || !p_backNeighbor;
	}

	glm::ivec2 ChunkStack::getChunkStackPosition() const {
		return chunkStackCoords;
	}

	Chunk* const ChunkStack::getChunk(std::uint8_t chunkCoordY) {
		return chunkData[chunkCoordY];
	}

	const std::array<Chunk*, ChunkStack::AMOUNT_OF_CHUNKS>& ChunkStack::getChunks() {
		return chunkData;
	}

	void ChunkStack::placeChunk(std::uint8_t chunkCoordY, Chunk* p_chunk) {
		chunkData[chunkCoordY] = p_chunk;
	}

	void ChunkStack::clearShouldSaveFlag() {
		shouldSaveFlag = false;
	}

	void ChunkStack::setShouldSaveFlag() {
		shouldSaveFlag = true;
	}

	void ChunkStack::possiblySavePersistant() {
		if(shouldSaveFlag) {
			shouldSaveFlag = false;
			level.getChunkIO().saveChunkStack(*this);
		}
	}

	bool ChunkStack::isDisabled() const {
		return field_isDisabled;
	}

	float ChunkStack::computeDistanceToPoint(float x, float z) const {
		glm::vec3 directionVector(
			(convertChunkToWorldValue(chunkStackCoords.x, Chunk::CHUNK_SIZE / 2) - x),
			0,
			(convertChunkToWorldValue(chunkStackCoords.y, Chunk::CHUNK_SIZE / 2) - z)
		);
		return glm::length(directionVector);
	}

	float ChunkStack::computeDistanceToPoint(const glm::vec2& point) const {
		return computeDistanceToPoint(point.x, point.y);
	}


}

