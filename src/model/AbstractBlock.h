#pragma once

#include "BlockType.h"
#include "TextureOrientation.h"
#include <cstdint>

namespace vc {
	class AbstractBlock {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			uint8_t inChunkX, inChunkY, inChunkZ;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			AbstractBlock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ);
			AbstractBlock();

		public:
			AbstractBlock(const AbstractBlock&) = delete;
			virtual ~AbstractBlock() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			void initAbstractBlock(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ);

		public:
			uint8_t getInChunkY() const;
			uint8_t getInChunkZ() const;
			uint8_t getInChunkX() const;

			virtual int getWorldX() const =0;
			virtual int getWorldY() const =0;
			virtual int getWorldZ() const =0;

			virtual int8_t getMetadata() const =0;
			virtual const BlockType& getBlockType() const =0;
			virtual const TextureOrientation& getTexOrientation() const =0;
			virtual float getWidth() const =0;
			virtual float getHeight() const =0;
			virtual float getDepth() const =0;
			virtual bool isSolid() const =0;


	};
}
