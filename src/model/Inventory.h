#pragma once

#include <vector>
#include "Slot.h"

namespace vc {
	class Inventory {
		public:
			class InventorySlot : public Slot {
				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					void click(Player& p);

			};

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int slotAmount;
			std::vector<InventorySlot> items;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			Inventory(int slotAmount);
			Inventory(const Inventory&) = delete;
			Inventory& operator=(const Inventory&) = delete;
			virtual ~Inventory() = default;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void click(int slot, Player& p);
			virtual void clear();
			virtual void clear(int slot);
			virtual void set(int slot, const std::shared_ptr<GameItem>& item, int amount);

			virtual const InventorySlot& get(int slot) const;
			virtual int length() const;
	};
}