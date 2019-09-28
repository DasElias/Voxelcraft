#pragma once

#include <memory>
#include "..//model/BlockType.h"
#include "FreePositionableBlockRenderer.h"
#include "../model/ProjectionMatrix.h"

namespace vc {
	class BlockInInventoryRenderer : FreePositionableBlockRenderer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private: 
			ProjectionMatrix blockInInventoryProjectionMatrix;
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			BlockInInventoryRenderer();
			BlockInInventoryRenderer(const BlockInInventoryRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(const std::shared_ptr<BlockType>&, const TextureArray& blockTextures, float absXMargin, float absYMargin, float width, float height);
	};
}	