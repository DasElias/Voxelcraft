#include "GameItemInInventoryRenderer.h"

namespace vc {
	void GameItemInInventoryRenderer::render(std::shared_ptr<GameItem> item, const TextureArray& blockTextures, float absXMargin, float absYMargin, float width, float height) {
		if(item == nullptr) return;

		if(item->isBlock()) {
			std::shared_ptr<BlockType> derived = std::dynamic_pointer_cast<BlockType>(item);
			if(! derived) throw std::runtime_error("The passed item pretends to be the type of a block, but in fact, it isn't");
			blockRenderer.render(derived, blockTextures, absXMargin, absYMargin, width, height);
		} else {
			throw std::logic_error("GameItems, which are not of type BlockType, are not supported yet");
		}

	}

}