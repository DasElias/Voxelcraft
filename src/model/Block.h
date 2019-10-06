#pragma once
#include "Chunk.h"
#include "Face.h"
#include "events/block/BlockBreakEvent.h"
#include "BlockType.h"
#include "AbstractBlock.h"

namespace vc {
	class Chunk;
	class Player;

	class Block : public AbstractBlock {
		
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			const BlockType* blockType = nullptr;
			const TextureOrientation* texOrientation = nullptr;
			Chunk* chunk = nullptr;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Block(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, const BlockType* blockType, Chunk& c, const TextureOrientation& texOrientation);
			Block();
			Block(const Block&) = delete;
			virtual ~Block() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------	
		private:
			float checkIntersectionForPlane(const glm::vec3& position, const glm::vec3& spanVector1, const glm::vec3& spanVector2, const glm::vec3& rayStartVector, const glm::vec3& rayDirectionVector) const;

		public:
			void initBlock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, const BlockType* blockType, Chunk& c, const TextureOrientation& texOrientation);
			std::string getMetadata() const override;
			const BlockType& getBlockType() const override;
			float getWidth() const override;
			float getHeight() const override;
			float getDepth() const override;
			bool isSolid() const override;
			const TextureOrientation& getTexOrientation() const override;
			tl::optional<std::pair<float, Face>> checkIntersectionAndGetFace() const;
			bool isInFrustum() const;

			int getWorldX() const override;
			int getWorldY() const override;
			int getWorldZ() const override;

			virtual void onDestroy(BlockBreakEvent& evt);
			virtual void onFocusUpdate(bool wasBlockPlaced);
			virtual void onUpdate();

			void cleanUp();
	};
}
