
#include "CreativeInventoryGUI.h"
#include <model\nodes\UnorganizedParentElement.h>
#include <model\nodes\VBox.h>


#include <model/nodeComponents/background/ColoredBackground.h>
#include "../../model/utils/FileUtils.h"
#include <model/positioning/CenterAllInParentWrapper.h>
#include <model/positioning/RelativePositioningInParent.h>
#include "../../model/Player.h"


using namespace egui;

namespace vc {
	CreativeInventoryGUI::CreativeInventory::CreativeInventory() :
			Inventory(std::max(int(std::ceilf(BlockType::getAll().size()) / 9) * 9, 5 * 9)) {

		// get map with all block types
		const std::map<int, std::shared_ptr<BlockType>>& allBlockTypes = BlockType::getAll();
		std::vector<std::shared_ptr<BlockType>> blockTypeList;

		// convert values into list
		for(auto& pair : allBlockTypes) {
			blockTypeList.push_back(pair.second);
		}

		// sort list
		std::sort(blockTypeList.begin(), blockTypeList.end(), [](std::shared_ptr<BlockType>& a, std::shared_ptr<BlockType>& b) {
			return a->getId() < b->getId();
		});

		// add BlockTypes to inventory
		for(int counter = 0; counter < blockTypeList.size(); counter++) {
			Inventory::set(counter, blockTypeList[counter], 1);
		}
	}

	void CreativeInventoryGUI::CreativeInventory::click(int slot, Player& p) {
		Slot& itemClipboard = p.getItemClipboard();
		if(itemClipboard.hasValue()) {
			// if the player has an item in his clipboard, no operation should be performed in creative inventory
			return;
		}

		std::shared_ptr<GameItem> previousInvItem = Inventory::get(slot).getGameItem();
		int previousInvItemAmount = Inventory::get(slot).getAmount();

		Inventory::click(slot, p);

		Inventory::set(slot, previousInvItem, previousInvItemAmount);
	}

	CreativeInventoryGUI::CreativeInventoryGUI() {
		static std::shared_ptr<Image> img = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\creativeInventory.png"));
		this->invElementBackground = img;
		
		float imgWidth = img->getWidth();
		float imgHeight = img->getHeight();
		float ar = imgWidth / imgHeight;
		invElement = std::shared_ptr<AspectRatioElement>(new AspectRatioElement(ar));
		
		invElement->setBackground(std::shared_ptr<Background>(
			new TexturedBackground(
				img
			)
		));


		invElement->setMaxDimension(0.8f, true, 0.8f, true);
		invElement->setOwnPositioning(std::shared_ptr<Positioning>(new egui::CenterAllInParentWrapper(std::shared_ptr<Positioning>(new RelativePositioningInParent(0, 0)))));
		inventoryGUI.setRootElement(invElement);
	}

	void CreativeInventoryGUI::render(Player& p, egui::MasterRenderer& eguiRenderer, const TextureArray& texArray) {
		eguiRenderer.beginFrame();
		inventoryGUI.render(eguiRenderer);
		eguiRenderer.endFrame();

		static GameItemInInventoryRenderer inInventoryRenderer = {};
		GameItemInInventoryRenderer& nonStaticInventoryRenderer = inInventoryRenderer;
		Inventory& playerInventory = p.getInventory();
		CreativeInventory& creativeInventory = creativeInv;

		iterateOverSlots([&playerInventory, &nonStaticInventoryRenderer, &texArray, &creativeInventory](int slotNumber, float beginX, float endX, float beginY, float endY) {
			const Slot& invSlot = (slotNumber < 9) ? playerInventory.get(slotNumber) : creativeInventory.get(slotNumber - 9);
			if(invSlot.hasValue()) {
				float const slotWidth = endX - beginX;
				float const slotHeight = endY - beginY;
				float const factor = 0.1;
				nonStaticInventoryRenderer.render(invSlot.getGameItem(), texArray, beginX + (slotWidth * factor), beginY + (slotHeight * factor), slotWidth - (slotWidth * 2 * factor), slotHeight - (slotHeight * 2 * factor));
			}
		});

		if(p.getItemClipboard().hasValue()) {
			float const guiWidth = invElement->getComputedWidth();
			float const guiHeight = invElement->getComputedHeight();
			float const slotWidth = (float(SLOT_DIMENSIONS) / invElementBackground->getWidth()) * guiWidth;
			float const slotHeight = (float(SLOT_DIMENSIONS) / invElementBackground->getHeight()) * guiHeight;
			float const absXMargin = x_pixelToPercent(egui::getCursorHandler().getCursorX());
			float const absYMargin = y_pixelToPercent(egui::getCursorHandler().getCursorY());

			float const factor = 0.1f;
			float finalWidth = slotWidth * (1 - 2 * factor);
			float finalHeight = slotHeight * (1 - 2 * factor);
			float finalXMargin = absXMargin - finalWidth / 2;
			float finalYMargin = absYMargin - finalHeight / 2;
			

			inInventoryRenderer.render(p.getItemClipboard().getGameItem(), texArray, finalXMargin, finalYMargin, finalWidth, finalHeight, 0.5f);
		}
	}

	void vc::CreativeInventoryGUI::iterateOverSlots(std::function<void(int, float, float, float, float)> callback) {
		float const guiXMargin = invElement->getAbsXMargin();
		float const guiYMargin = invElement->getAbsYMargin();
		float const guiWidth = invElement->getComputedWidth();
		float const guiHeight = invElement->getComputedHeight();

		float const c_marginLeft = (float(MARGIN_LEFT) / invElementBackground->getWidth()) * guiWidth;
		float const c_marginTop = (float(MARGIN_TOP) / invElementBackground->getHeight()) * guiHeight;
		float const c_slotWidth = (float(SLOT_DIMENSIONS) / invElementBackground->getWidth()) * guiWidth;
		float const c_slotHeight = (float(SLOT_DIMENSIONS) / invElementBackground->getHeight()) * guiHeight;
		float const c_xSpaceBetween = (float(SPACE_BETWEEN_SLOTS) / invElementBackground->getWidth()) * guiWidth;
		float const c_ySpaceBetween = (float(SPACE_BETWEEN_SLOTS) / invElementBackground->getHeight()) * guiHeight;
		float const c_spaceToHotbar = (float(SPACE_TO_HOTBAR) / invElementBackground->getHeight()) * guiHeight;

		float rowYBegin[6] = {
			guiYMargin + c_marginTop + 5 * c_slotHeight + (4 + 1) * c_ySpaceBetween + c_spaceToHotbar,
			guiYMargin + c_marginTop + 4 * c_slotHeight + (4 + 1) * c_ySpaceBetween,
			guiYMargin + c_marginTop + 3 * c_slotHeight + (3 + 1) * c_ySpaceBetween,
			guiYMargin + c_marginTop + 2 * c_slotHeight + (2 + 1) * c_ySpaceBetween,
			guiYMargin + c_marginTop + 1 * c_slotHeight + (1 + 1) * c_ySpaceBetween,
			guiYMargin + c_marginTop + 0 * c_slotHeight + (0 + 1) * c_ySpaceBetween,
		};

		for(int row = 0; row < 6; row++) {
			float const rowBegin = rowYBegin[row];
			float const rowEnd = rowBegin + c_slotHeight;

			for(int col = 0; col < 9; col++) {
				float const colBegin = guiXMargin + c_marginLeft + (col * c_slotWidth) + (col * c_xSpaceBetween);
				float const colEnd = colBegin + c_slotWidth;
				int const slot = row * 9 + col;

				callback(slot, colBegin, colEnd, rowBegin, rowEnd);
			}
		}
	}

	void CreativeInventoryGUI::click(Player& p, float percentX, float percentY, bool isDownNow) {
		std::array<bool, 9 * (5 + 1)>& areSlotsClicked = this->areSlotsClicked;
		PlayerInventory& playerInventory = p.getInventory();
		CreativeInventory& creativeInventory = creativeInv;

		iterateOverSlots([&areSlotsClicked, &p, &playerInventory, &creativeInventory, percentX, percentY, isDownNow](int slotNumber, float beginX, float endX, float beginY, float endY) {
			if(! isDownNow && areSlotsClicked[slotNumber]) {
				if(percentX >= beginX && percentX <= endX && percentY >= beginY && percentY <= endY) {			
					if(slotNumber < 9) {
						playerInventory.click(slotNumber, p);
					} else {
						creativeInventory.click(slotNumber - 9, p);
					}
					
				}
			}			

			areSlotsClicked[slotNumber] = isDownNow;

		});
	}

	

}