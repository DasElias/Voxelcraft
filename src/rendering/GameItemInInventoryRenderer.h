#pragma once

#include "BlockInInventoryRenderer.h"

namespace vc {
	class GameItemInInventoryRenderer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			BlockInInventoryRenderer blockRenderer;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			GameItemInInventoryRenderer() = default;
			GameItemInInventoryRenderer(const GameItemInInventoryRenderer&) = delete;
			GameItemInInventoryRenderer& operator=(const GameItemInInventoryRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(std::shared_ptr<GameItem> item, const TextureArray& blockTextures, float absXMargin, float absYMargin, float width, float height, float opacity = 1);
	};
}