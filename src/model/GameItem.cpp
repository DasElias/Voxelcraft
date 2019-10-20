#include "GameItem.h"
#include <stdexcept>

namespace vc {
	std::map<int, std::shared_ptr<GameItem>> GameItem::ALL_ITEM_TYPES = {};


	void GameItem::addGameItem(const std::shared_ptr<GameItem>& itemType) {
		if(ALL_ITEM_TYPES.find(itemType->getId()) != ALL_ITEM_TYPES.end()) {
			throw std::runtime_error("this id does already exist.");
		} else {
			ALL_ITEM_TYPES.insert(std::make_pair(itemType->getId(), itemType));
		}
	}

	const std::shared_ptr<GameItem>& GameItem::getGameItemById(int id) {
		try {
			return ALL_ITEM_TYPES.at(id);
		} catch(std::out_of_range ex) {
			throw std::runtime_error("this id doesn't exist");
		}
	}


	const std::map<int, std::shared_ptr<GameItem>>& GameItem::getAll() {
		return ALL_ITEM_TYPES;
	}
}