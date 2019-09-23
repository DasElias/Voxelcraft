#include "Slot.h"
#include <stdexcept>
#include <string>

namespace vc {
	Slot::Slot(std::shared_ptr<GameItem> item, int amount) :
			gameItem(item),
			amount(amount) {
	}

	void Slot::clear() {
		this->amount = 0;
		this->gameItem = {};
	}

	void Slot::set(const Slot& other) {
		set(other.gameItem, other.amount);
	}

	void Slot::set(const std::shared_ptr<GameItem>& i, int amount) {
		if(! i && amount != 0) throw std::invalid_argument("Can't set an amount greater than 0 for an invalid item type.");

		this->gameItem = i;
		setAmount(amount);
	}

	void Slot::setAmount(int amount) {
		if(amount < 0) throw std::invalid_argument("Invalid argument for amount: " + std::to_string(amount));
		if(amount == 0) clear();

		this->amount = amount;
	}

	void Slot::swap(Slot& other) {
		std::shared_ptr<GameItem> thisGameItem = this->gameItem;
		int thisAmount = this->amount;

		set(other);
		other.set(thisGameItem, thisAmount);
	}
	bool Slot::hasValue() const {
		// we can't use "return gameItem;"
		return ((gameItem) ? true : false) && amount != 0;
	}
	std::shared_ptr<GameItem> Slot::getGameItem() const {
		return gameItem;
	}
	int Slot::getAmount() const {
		return amount;
	}
}
