#pragma once

#include <array>

#include "GameItemInInventoryRenderer.h"
#include "../model/PlayerInventory.h"

#include <model\scene\Scene.h>

#include <model/nodes/Label.h>
#include <model/nodes/HBox.h>
#include <model/positioning/RelativePositioningInParent.h>
#include <model/positioning/CenterXInParentWrapper.h>
#include <model/nodeComponents/background/LinearGradientBackground.h>

namespace vc {
	class IngameHotbarRenderer : public GameItemInInventoryRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const BOX_DIMENSIONS = 65;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TextureArray blockTextureArray;
			
			egui::Scene scene;
			std::array<std::shared_ptr<egui::Label>, 9> fields;

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
			void renderHotbar(PlayerInventory& playerInventory);

	};
}