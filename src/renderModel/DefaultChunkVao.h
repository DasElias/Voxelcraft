#pragma once

#include "ChunkVertexArrayObject.h"
#include "../model/Chunk.h"

namespace vc {
	namespace renderingModel {
		class DefaultChunkVao : public ChunkVertexArrayObject {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
		private:
			static int const DATA_LENGTH = vc::model::Chunk::CHUNK_SIZE * vc::model::Chunk::CHUNK_SIZE * vc::model::Chunk::CHUNK_SIZE * 6 * 3 / 2;
			static int const TEXTUREID_FACING_LENGTH = vc::model::Chunk::CHUNK_SIZE * vc::model::Chunk::CHUNK_SIZE * vc::model::Chunk::CHUNK_SIZE * 6 / 2;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
		private:
			std::uint32_t posDataVboId = 0;
			std::uint32_t texFacingVboId = 0;

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
			void updateData(std::vector<float>& posData, std::vector<uint16_t>& texFacingData) override;
			
			int getPosDataLength() const override;
			int getTexFacingDataLength() const override;
		};
	}
}
