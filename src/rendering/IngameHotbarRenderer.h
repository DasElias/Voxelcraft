#pragma once

#include <array>

#include "GameItemInInventoryRenderer.h"
#include "../model/PlayerInventory.h"
#include "Renderer2D.h"
#include "..//renderModel/SimpleTexture.h"

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
			static int const SPRITESHEET_DIMENSIONS = 1024;

			static int const HOTBAR_SPRITE_WIDTH = 722;
			static int const HOTBAR_SPRITE_HEIGHT = 82;
			static int const HOTBAR_SPRITE_BORDER = 1;
			static int const HOTBAR_BOX_BORDER = 2;
			static int const HOTBAR_BOX_DIMENSIONS = (HOTBAR_SPRITE_WIDTH - (2 * HOTBAR_SPRITE_BORDER) - (9 * 2 * HOTBAR_BOX_BORDER)) / 9;

			static int const ACTIVEELEM_MARGINY = 90;
			static int const ACTIVEELEM_SPRITE_DIMENSIONS = 80;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TextureArray blockTextureArray;
			
			std::shared_ptr<egui::Label> hotbarElement;
			SimpleTexture hotbarTextureSheet;

			std::shared_ptr<egui::RelativePositioningOnScreen> selectedElemPositioning;
			std::shared_ptr<egui::Label> selectedElemLabel;

			egui::MasterRenderer& eguiRenderer;
			egui::EGuiContext& ctx;

			Renderer2D& renderer2D;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IngameHotbarRenderer(TextureArray arr, Renderer2D& renderer2D, egui::MasterRenderer& renderer, egui::EGuiContext& ctx);
			IngameHotbarRenderer(const IngameHotbarRenderer&) = delete;
			IngameHotbarRenderer& operator=(const IngameHotbarRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderHotbar(PlayerInventory& playerInventory, int selectedSlot);

	};
}