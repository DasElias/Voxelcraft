#pragma once

#include <array>
#include "ChunkStack.h"
#include "Level.h"
#include <vector>

#include "../renderModel/ChunkVertexArrayObject.h"
#include "AbstractChunk.h"
#include "Block.h"
#include "../renderModel/ChunkVaoData.h"


namespace vc {
	class Player;
	class Level;
	class ChunkStack;

	class Chunk : public AbstractChunk {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const CHUNK_SIZE = 40;
			static float const CHUNK_SIZE_FLOAT;
			static int const CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			glm::ivec3 const chunkCoords;
			Level& level;
			ChunkStack& chunkStack;

			std::array<Block*, CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE> blocks = {};
			int placedBlockCounter = 0;
			ChunkVertexArrayObject* p_chunkVaoObject;
			int maxHeight = 0;
			float squaredDistanceToPlayer = 0;
			bool lastTimeVisible = true;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Chunk(glm::ivec3 chunkCoords, Level& lvl, ChunkStack& chunkStack);
			Chunk(const Chunk&) = delete;
			~Chunk();


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void createBlock(Block* p_block, uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, std::vector<ChunkVaoData>& data);
			void addToDataArray(std::vector<ChunkVaoData>& data, Block& block, std::int8_t facing, std::int8_t tex);

		public:
			bool wasLastTimeVisible() const override;
			void setLastTimeVisible(bool lastTimeVisible) override;

			float getSquaredDistanceToPlayer() const override;
			void updateDistanceToPlayer() override;

			int getMaxHeight() const override;
			ChunkVertexArrayObject* const getVao() const override;
			int getAmountOfPlacedBlocks() const override;

			void updateVao() override;

			void placeBlockWithoutUpdateAndEvent(Block* p_block, uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ);
			void placeBlock(Block* p_block, uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, Player* p_triggeringPlayer, bool fireEvent = true, bool updateVao = true);

			float computeDistanceToPointSquared(float x, float y, float z) const override;
			float computeDistanceToPointSquared(const glm::vec3& point) const override;
			float computeDistanceToPoint(float x, float y, float z) const override;
			float computeDistanceToPoint(const glm::vec3& point) const override;

			bool isInFrustum() const override;
			glm::ivec3 getChunkCoordinates() const override;
			Block* const getBlock(uint8_t intChunkX, uint8_t inChunkY, uint8_t intChunkZ);
			Level& getLevel();
	};
}
