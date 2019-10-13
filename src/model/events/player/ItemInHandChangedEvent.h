#pragma once

#include "PlayerEvent.h"
#include <model/events/Cancellable.h>


namespace vc {
	class GameItem;

	class ItemInHandChangedEvent : public PlayerEvent, public egui::Cancellable {

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<GameItem> const oldItem;
			std::shared_ptr<GameItem> const newItem;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ItemInHandChangedEvent(AbstractPlayer* const p_player, const std::shared_ptr<GameItem>& oldItem, const std::shared_ptr<GameItem>& newItem);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::shared_ptr<GameItem> getOldItem() const;
			std::shared_ptr<GameItem> getNewItem() const;
	};
}
