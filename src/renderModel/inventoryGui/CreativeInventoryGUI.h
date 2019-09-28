
#pragma once
#include "InventoryGUI.h"
#include "..//..//model/Inventory.h"
#include <model\scene\Scene.h>
#include <model\nodes\AspectRatioElement.h>
#include <model/nodeComponents/background/TexturedBackground.h>
#include <array>

namespace vc {
	class CreativeInventoryGUI : public InventoryGUI {
		private:
			class CreativeInventory : public Inventory {
				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					CreativeInventory();

				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					void click(int slot, Player& p) override;
			};

		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const MARGIN_LEFT = 33;
			static int const MARGIN_TOP = 69;
			static int const SLOT_DIMENSIONS = 68;
			static int const SPACE_BETWEEN_SLOTS = 4;
			static int const SPACE_TO_HOTBAR = 20;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::Scene inventoryGUI;
			std::shared_ptr<egui::AspectRatioElement> invElement;
			std::shared_ptr<egui::Image> invElementBackground;

			CreativeInventory creativeInv;

			std::array<bool, 9 * (5 + 1)> areSlotsClicked = {false};

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreativeInventoryGUI();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			// params: slotNumber, beginX, endX, beginY, endY
			void iterateOverSlots(std::function<void(int, float, float, float, float)> callback);
		
		public:
			void render(Player& p, egui::MasterRenderer& eguiRenderer, const TextureArray& texArray) override;
			void click(Player& p, float percentX, float percentY, bool isDownNow) override;
	};
}