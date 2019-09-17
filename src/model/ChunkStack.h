#pragma once

#include <glm/vec2.hpp>
#include "Level.h"
#include "AbstractChunkStack.h"

namespace vc {
	class Level;
	class Chunk;

	class ChunkStack : public AbstractChunkStack {
		// ----------------------------------------------------------------------
		// -----------------------------CHILD-CLASS------------------------------
		// ----------------------------------------------------------------------
		public:
			class NeighborAccessKey {
					private:
						NeighborAccessKey() = default;

					friend class Level;
					friend class ChunkStack;
					friend class ChunkIO;
				};

		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const AMOUNT_OF_CHUNKS = 2;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool isFirstInitialized = false;
			bool field_isDisabled = true;
			ChunkStack* p_leftNeighbor = nullptr;
			ChunkStack* p_rightNeighbor = nullptr;
			ChunkStack* p_frontNeighbor = nullptr;
			ChunkStack* p_backNeighbor = nullptr;
			glm::ivec2 chunkStackCoords;
			Level& level;
			std::array<Chunk*, AMOUNT_OF_CHUNKS> chunkData;

			bool shouldSaveFlag = false;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ChunkStack(glm::ivec2 chunkStackCoords, Level& level);
			~ChunkStack();
			ChunkStack(const ChunkStack&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void setNeighbour();
			void updateDisabledFlag();

		public:
			void setFirstInitialized(NeighborAccessKey key);
			void setLeftNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor);
			void setRightNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor);
			void setFrontNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor);
			void setBackNeighbor(NeighborAccessKey key, ChunkStack* p_neighbor);
			ChunkStack* const getLeftNeighbor() const;
			ChunkStack* const getRightNeighbor() const;
			ChunkStack* const getFrontNeighbor() const;
			ChunkStack* const getBackNeighbor() const;
			bool isAtLeastOneNeighborNull() const;

			glm::ivec2 getChunkStackPosition() const override;
			Chunk* const getChunk(std::uint8_t chunkCoordY);
			const std::array<Chunk*, AMOUNT_OF_CHUNKS>& getChunks();
			void placeChunk(std::uint8_t chunkCoordY, Chunk* p_chunk);

			void clearShouldSaveFlag();
			void setShouldSaveFlag();
			void possiblySavePersistant();

			bool isDisabled() const override;
			float computeDistanceToPoint(float x, float z) const override;
			float computeDistanceToPoint(const glm::vec2& point) const override;
	};

}
