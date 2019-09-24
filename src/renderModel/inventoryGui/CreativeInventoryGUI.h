#pragma once
#include "InventoryGUI.h"
#include <model\scene\Scene.h>

namespace vc {
	class CreativeInventoryGUI : public InventoryGUI {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::Scene inventoryGUI;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreativeInventoryGUI();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(Player& p, egui::MasterRenderer& eguiRenderer, const TextureArray& texArray) override;
			void click(Player& p, float percentX, float percentY) override;
	};
}