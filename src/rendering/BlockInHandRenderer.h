#pragma once

#include "FreePositionableBlockRenderer.h"
#include "BasicInHandRenderer.h"

namespace vc {
	class BlockInHandRenderer : FreePositionableBlockRenderer, public BasicInHandRenderer {
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