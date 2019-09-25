
#include "IngameHotbarRenderer.h"
#include "../model/utils/FileUtils.h"
#include <model/utils/PositioningUtils.h>
#include "..//model/Player.h"

#include <model/positioning/RelativePositioningInParent.h>


namespace vc {
	IngameHotbarRenderer::IngameHotbarRenderer(TextureArray arr, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& ctx) :
			blockTextureArray(arr),
			eguiRenderer(eguiRenderer),
			ctx(ctx) {

		// set label for selected element
		this->selectedElemLabel = std::shared_ptr<egui::Label>(new egui::Label());
		this->selectedElemScene.setRootElement(selectedElemLabel);

		this->selectedElemPositioning = std::shared_ptr<egui::RelativePositioningOnScreen>(new egui::RelativePositioningOnScreen(0, 0));

		// set dimensions and positioning
		selectedElemLabel->setPreferredWidth(0, false);
		selectedElemLabel->setPreferredHeight(0, false);
		selectedElemLabel->setOwnPositioning(selectedElemPositioning);

		// load background image for selected element
		std::shared_ptr<egui::Image> selectedElemBg = egui::Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\selectedSlot.png"));
		selectedElemLabel->setBackground(std::shared_ptr<egui::Background> (
			new egui::TexturedBackground(
				selectedElemBg
			)
		));


		// set label for hotbar background
		std::shared_ptr<egui::Label> hotbarElem(new egui::Label());
		this->hotbarBackgroundScene.setRootElement(hotbarElem);
		hotbarElem->setPreferredDimension(WHOLE_IMG_WIDTH, false, WHOLE_IMG_HEIGHT, false);


		// set background for hotbar 
		fieldBackgroundImage = egui::Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\hotbar.png"));
		std::shared_ptr<egui::Background> bg(
			new egui::TexturedBackground(
				fieldBackgroundImage
			)
		);
		hotbarElem->setBackground(bg);

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
		hotbarElem->setOwnPositioning(pos);
	}

	void IngameHotbarRenderer::renderHotbar(PlayerInventory& playerInventory, int selectedSlot) {
		eguiRenderer.beginFrame();
		hotbarBackgroundScene.render(eguiRenderer);
		eguiRenderer.endFrame();

		double const imgWidth = fieldBackgroundImage->getWidth();
		double const imgHeight = fieldBackgroundImage->getHeight();
		double const elemWidth = hotbarBackgroundScene.getRootElement()->getComputedWidth();
		double const elemHeight = hotbarBackgroundScene.getRootElement()->getComputedHeight();
		double const elemMarginX = hotbarBackgroundScene.getRootElement()->getAbsXMargin();
		double const elemMarginY = hotbarBackgroundScene.getRootElement()->getAbsYMargin();

		double const imgBorderWidth = (WHOLE_IMG_BORDER / imgWidth) * elemWidth;
		double const imgBorderHeight = (WHOLE_IMG_BORDER / imgHeight) * elemHeight;

		double const slotBorderWidth = (SINGLE_BOX_BORDER / imgWidth) * elemWidth;
		double const slotBorderHeight = (SINGLE_BOX_BORDER / imgHeight) * elemHeight;

		double const slotWidth = (SINGLE_BOX_DIMENSIONS / imgWidth) * elemWidth;
		double const slotHeight = (SINGLE_BOX_DIMENSIONS / imgHeight) * elemHeight;

		for(int counter = 0; counter < 9; counter++) {
			const Slot& invSlot = playerInventory.get(counter);
			if(! invSlot.hasValue()) continue;

			/*
			 * counter + 1	=> the for-loop starts counting at 0, but already for the first element we have to add 1 horizontal border to absXMargin
			 * * 2			=> every slot has one border at the left side and one at the right side
			 * - 1			=> we don't have to add the border behind the current slot to absXMargin
			 */
			int horizontalSlotBorderAmount = (counter + 1) * 2 - 1;

			double absXMargin = elemMarginX + imgBorderWidth + horizontalSlotBorderAmount * slotBorderWidth + counter * slotWidth;
			double absYMargin = elemMarginY + imgBorderHeight + slotBorderHeight;

			GameItemInInventoryRenderer::render(
				invSlot.getGameItem(),
				blockTextureArray,
				absXMargin + (0.1 * slotWidth),
				absYMargin + (0.1 * slotHeight),
				slotWidth - (0.2 * slotWidth),
				slotHeight - (0.2 * slotHeight)
			);
		}

		// render marker for active element
		int horizontalSlotBorderAmount = 2 * selectedSlot;
		double absXMargin = elemMarginX + imgBorderWidth + horizontalSlotBorderAmount * slotBorderWidth + selectedSlot * slotWidth;
		double absYMargin = elemMarginY + imgBorderHeight + slotBorderHeight;

		double width = slotWidth + 2 * slotBorderHeight;
		double height = slotHeight + 2 * slotBorderHeight;

		selectedElemLabel->setPreferredDimension(width, true, height, true);
		selectedElemPositioning->setX(absXMargin);
		selectedElemPositioning->setY(absYMargin);

		eguiRenderer.beginFrame();
		selectedElemScene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
}