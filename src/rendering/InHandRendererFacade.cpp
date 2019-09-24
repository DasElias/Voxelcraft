#include "InHandRendererFacade.h"

namespace vc {
	InHandRendererFacade::InHandRendererFacade(TextureArray& texArray) :
			blockRenderer(texArray) {
	}

	void InHandRendererFacade::render(std::shared_ptr<GameItem> item) {
		if(item == nullptr) return;

		if(item->isBlock()) {
			std::shared_ptr<BlockType> derived = std::dynamic_pointer_cast<BlockType>(item);
			if(!derived) throw std::runtime_error("The passed item pretends to be the type of a block, but in fact, it isn't");
			blockRenderer.render(derived);
			
		} else {
			throw std::logic_error("GameItems, which are not of type BlockType, are not supported yet");
		}
	}
	void InHandRendererFacade::setCurrentLevel(Level* level) {
		blockRenderer.setCurrentLevel(level);
	}
}
