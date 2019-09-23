#include "BlockInHandRenderer.h"



namespace vc {
	BlockInHandRenderer::BlockInHandRenderer(TextureArray blockTextureArray) :
			blockTextureArray(blockTextureArray) {
	}

	void BlockInHandRenderer::render(const std::shared_ptr<BlockType>& type) {
		FreePositionableBlockRenderer::render(type, blockTextureArray, getModelProjectionMatrix());
	}
}
