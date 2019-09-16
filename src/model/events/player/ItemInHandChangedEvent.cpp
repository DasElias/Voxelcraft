#include "ItemInHandChangedEvent.h"

namespace vc::model {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	ItemInHandChangedEvent::ItemInHandChangedEvent( AbstractPlayer* const p_player, const std::shared_ptr<BlockType>& oldItem, const std::shared_ptr<BlockType>& newItem) :
			PlayerEvent(p_player),
			oldItem(oldItem),
			newItem(newItem) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	std::shared_ptr<BlockType> ItemInHandChangedEvent::getOldItem() const {
		return oldItem;
	}

	std::shared_ptr<BlockType> ItemInHandChangedEvent::getNewItem() const {
		return newItem;
	}


}

