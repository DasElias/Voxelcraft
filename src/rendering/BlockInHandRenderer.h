#pragma once

#include "FreePositionableBlockRenderer.h"

namespace vc {
	class BlockInHandRenderer : FreePositionableBlockRenderer, public InHandRenderer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TextureArray blockTextureArray;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			BlockInHandRenderer(TextureArray blockTextureArray);
			BlockInHandRenderer(const BlockInHandRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(const std::shared_ptr<BlockType>& type);
	};
}