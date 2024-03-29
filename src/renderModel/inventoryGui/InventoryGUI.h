#pragma once
#include "../TextureArray.h"

#include "..//..//rendering/GameItemInInventoryRenderer.h"

namespace egui {
	class MasterRenderer;
}

namespace vc {
	class Player;

	class InventoryGUI {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			GameItemInInventoryRenderer itemRenderer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			InventoryGUI() = default;
			InventoryGUI(const InventoryGUI&) = delete;
			InventoryGUI& operator=(const InventoryGUI&) = delete;
			virtual ~InventoryGUI() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void render(Player& p, egui::MasterRenderer& eguiRenderer, const TextureArray& texArray) =0;
			virtual void click(Player& p, float percentX, float percentY, bool isDownNow) =0;
			virtual void onGuiOpen(Player& p);
			virtual void onGuiClose(Player& p);
	};
}