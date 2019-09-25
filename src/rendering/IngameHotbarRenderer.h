#pragma once

#include <array>

#include "GameItemInInventoryRenderer.h"
#include "../model/PlayerInventory.h"

#include <model\scene\Scene.h>

#include <model/nodes/Label.h>
#include <model/nodes/HBox.h>
#include <model/positioning/RelativePositioningOnScreen.h>
#include <model/positioning/CenterXInParentWrapper.h>
#include <model/nodeComponents/background/TexturedBackground.h>

namespace vc {
	class IngameHotbarRenderer : public GameItemInInventoryRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const WHOLE_IMG_WIDTH = 722;
			static int const WHOLE_IMG_HEIGHT = 82;
			static int const WHOLE_IMG_BORDER = 1;
			static int const SINGLE_BOX_BORDER = 2;

			static int const SINGLE_BOX_DIMENSIONS = (WHOLE_IMG_WIDTH - (2 * WHOLE_IMG_BORDER) - (9 * 2 * SINGLE_BOX_BORDER)) / 9;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TextureArray blockTextureArray;
			
			egui::Scene hotbarBackgroundScene;
			std::shared_ptr<egui::Image> fieldBackgroundImage;

			egui::Scene selectedElemScene;
			std::shared_ptr<egui::RelativePositioningOnScreen> selectedElemPositioning;
			std::shared_ptr<egui::Label> selectedElemLabel;

			egui::MasterRenderer& eguiRenderer;
			egui::EGuiContext& ctx;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IngameHotbarRenderer(TextureArray arr, egui::MasterRenderer& renderer, egui::EGuiContext& ctx);
			IngameHotbarRenderer(const IngameHotbarRenderer&) = delete;
			IngameHotbarRenderer& operator=(const IngameHotbarRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderHotbar(PlayerInventory& playerInventory, int selectedSlot);

	};
}