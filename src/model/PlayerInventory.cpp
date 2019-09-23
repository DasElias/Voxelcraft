#include "PlayerInventory.h"

#include "BlockType.h"

namespace vc {
	PlayerInventory::PlayerInventory() :
			Inventory(4 * 9) {

		// use only for creative inventory
		Inventory::set(0, BlockType::GRASS, 1);
		Inventory::set(1, BlockType::DIRT, 1);
		Inventory::set(2, BlockType::STONE, 1);
		Inventory::set(3, BlockType::COBBLESTONE, 1);
		Inventory::set(4, BlockType::STONE_BRICKS, 1);
		Inventory::set(5, BlockType::OAK_LOG, 1);
		Inventory::set(6, BlockType::OAK_PLANKS, 1);
		Inventory::set(7, BlockType::BIRCH_LOG, 1);
		Inventory::set(8, BlockType::BIRCH_PLANKS, 1);
	}
}
