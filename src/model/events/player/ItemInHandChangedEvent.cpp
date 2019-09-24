#include "ItemInHandChangedEvent.h"

#include "..//..//GameItem.h"

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	ItemInHandChangedEvent::ItemInHandChangedEvent( AbstractPlayer* const p_player, const std::shared_ptr<GameItem>& oldItem, const std::shared_ptr<GameItem>& newItem) :
			PlayerEvent(p_player),
			oldItem(oldItem),
			newItem(newItem) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	std::shared_ptr<GameItem> ItemInHandChangedEvent::getOldItem() const {
		return oldItem;
	}

	std::shared_ptr<GameItem> ItemInHandChangedEvent::getNewItem() const {
		return newItem;
	}


}

