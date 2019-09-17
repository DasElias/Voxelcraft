#pragma once

#include "../../BlockType.h"
#include "PlayerEvent.h"
#include "../Cancellable.h"


namespace vc {
	class ItemInHandChangedEvent : public PlayerEvent, public Cancellable {

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<BlockType> const oldItem;
			std::shared_ptr<BlockType> const newItem;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ItemInHandChangedEvent(AbstractPlayer* const p_player, const std::shared_ptr<BlockType>& oldItem, const std::shared_ptr<BlockType>& newItem);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::shared_ptr<BlockType> getOldItem() const;
			std::shared_ptr<BlockType> getNewItem() const;
	};
}
