#pragma once

#include <memory>
#include "GameItem.h"



namespace vc {
	class Player;
	class Slot {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<GameItem> gameItem;
			int amount;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Slot(std::shared_ptr<GameItem> item = {}, int amount = 0);
			Slot(const Slot&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void clear();
			virtual void set(const Slot& other);
			virtual void set(const std::shared_ptr<GameItem>& i, int amount);
			virtual void setAmount(int amount);
			virtual void swap(Slot& other);

			virtual bool hasValue() const;
			virtual std::shared_ptr<GameItem> getGameItem() const;
			virtual int getAmount() const;
	};
}