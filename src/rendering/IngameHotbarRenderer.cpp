
#include "IngameHotbarRenderer.h"



namespace vc {
	IngameHotbarRenderer::IngameHotbarRenderer(TextureArray arr, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& ctx) :
			blockTextureArray(arr),
			eguiRenderer(eguiRenderer),
			ctx(ctx) {

		// create box for 9 labels
		std::shared_ptr<egui::HBox> fieldBox(new egui::HBox());
		fieldBox->setPreferredDimension(9 * BOX_DIMENSIONS, false, BOX_DIMENSIONS, false);

		// set root element
		scene.setRootElement(fieldBox);
		

		// create 9 labels
		for(int counter = 0; counter < fields.size(); counter++) {
			fields[counter] = std::shared_ptr<egui::Label>(new egui::Label());

			// set width of every box
			fields[counter]->setPreferredWidth(BOX_DIMENSIONS, false);

			// background for every box
			std::shared_ptr<egui::Background> defaultBoxBg(
				new egui::LinearGradientBackground(
					egui::LinearGradient(
						egui::Color(1, 0, 0),
						egui::Color(0, 1, 0),
						0, 0,
						1, 1
					)
				)
			);
			fields[counter]->setBackground(defaultBoxBg);

			fieldBox->addChildElement(fields[counter]);
		}

		// set positioning of hotbar
		class HotbarPositioning : public egui::RelativePositioningInParent {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				HotbarPositioning(float absXMargin_bottomLeft, float absYMargin_bottomLeft) :
						RelativePositioningInParent(absXMargin_bottomLeft, absYMargin_bottomLeft) {

				}

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				float getAbsYMargin(egui::PositionableElement& target) override {
					float r = egui::RelativePositioningInParent::getAbsYMargin(target);
					return r - target.getComputedHeight();
				}
		};

		std::shared_ptr<egui::Positioning> pos(new egui::CenterXInParentWrapper(std::shared_ptr<egui::Positioning>(new HotbarPositioning(0,1))));
		fieldBox->setOwnPositioning(pos);
	}

	void IngameHotbarRenderer::renderHotbar(PlayerInventory& playerInventory) {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		// since we want to render the hotbar, we have to use the first 9 slots
		for(int counter = 0; counter < 9; counter++) {
			const Slot& invSlot = playerInventory.get(counter);
			if(invSlot.hasValue()) {
				GameItemInInventoryRenderer::render(
					invSlot.getGameItem(),
					blockTextureArray,
					fields[counter]->getAbsXMargin(),
					fields[counter]->getAbsYMargin(),
					fields[counter]->getComputedWidth(),
					fields[counter]->getComputedHeight()
				);
			}
		}
	}
}