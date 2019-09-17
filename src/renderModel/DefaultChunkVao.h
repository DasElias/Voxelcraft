#pragma once

#include "ChunkVertexArrayObject.h"
#include "../model/Chunk.h"

namespace vc {
	class DefaultChunkVao : public ChunkVertexArrayObject {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
	private:
		static int const DATA_LENGTH = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * 6 * 3 / 2;
		static int const TEXTUREID_FACING_LENGTH = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * 6 / 2;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
	private:
		std::uint32_t dataVboId = 0;

		std::uint32_t vaoId = 0;
		std::size_t indiciesCount = 0;

		bool isInitialized = false;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
	public:
		DefaultChunkVao(bool init);
		DefaultChunkVao(const DefaultChunkVao&) = delete;
		~DefaultChunkVao();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
	private:
		void init();

	public:
		std::uint32_t getVertexArrayObjectId() override;
		std::size_t getIndiciesCount() override;

		void updateData(std::vector<ChunkVaoData>& data) override;
	
	};
}
