#include "Inventory.h"

#include "Player.h"

namespace vc {

	void Inventory::InventorySlot::click(Player& p) {
		if(p.getItemClipboard().hasValue()) {
			// item clipboard has value

			if(this->hasValue()) {
				// try to add items in clipboard to this slot

				if(this->getGameItem()->getName() == p.getItemClipboard().getGameItem()->getName()) {
					// name of item in clipboard is equal to name of item in this slot

					// type of both items
					std::shared_ptr<GameItem> type = this->getGameItem();

					// amount of items in inventoryslot
					int itemsInThisSlot = this->getAmount();

					// max stack size of type
					int maxItemsInSlot = type->getMaxStackSize();

					// amount of items in clipboard
					int itemsInClipboard = p.getItemClipboard().getAmount();

					// max amount of items, which can be added from clipboard to this slot
					int maxItemsToAdd = maxItemsInSlot - itemsInThisSlot;

					// actual amount of items, which are going to be added from clipboard to this list
					int actualItemsToAdd = std::min(maxItemsToAdd, maxItemsInSlot);

					// remaining items in clipboard
					int remainingAmountInClipboard = itemsInClipboard - actualItemsToAdd;


					/*
					 * set item amount in clipboard to remainingAmountInClipboard, if remainingAmountInClipboard
					 * is 0, the clipboard will be cleared automatically
					 */
					 p.getItemClipboard().setAmount(remainingAmountInClipboard);

					 /**
					  * set item amount in this slot
					  */
					  this->setAmount(this->getAmount() + actualItemsToAdd);
				} else {
					// swap values
					this->swap(p.getItemClipboard());
				}
			} else {
				// slot doesn't contain items, but the clipboard does so
				this->set(p.getItemClipboard());
			}
		} else if(this->hasValue()) {
			// item clipboard has no value, but this slot has one
			p.getItemClipboard().set(*this);
			this->clear();
		} else {
			/*
			 * We don't need to do anything if the clipboard is as well as empty as this slot.
			 */
		}
	}






	Inventory::Inventory(int slotAmount) :
			slotAmount(slotAmount),
			items(slotAmount) {
	}

	void Inventory::click(int slot, Player& p) {
		items.at(slot).click(p);
	}

	void Inventory::clear() {
		for(auto& s : items) {
			s.clear();
		}
	}

	void Inventory::clear(int slot) {
		items.at(slot).clear();
	}

	void Inventory::set(int slot, const std::shared_ptr<GameItem>& item, int amount) {
		items.at(slot).set(item, amount);
	}

	const Inventory::InventorySlot& Inventory::get(int slot) const {
		return items.at(slot);
	}

	int Inventory::length() const {
		return items.size();
	}
	
}
