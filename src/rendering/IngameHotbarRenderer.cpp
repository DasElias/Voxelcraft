
#include "IngameHotbarRenderer.h"
#include "../model/utils/FileUtils.h"
#include <model/utils/PositioningUtils.h>
#include "..//model/Player.h"
#include "..//renderModel/SimpleTexture.h"
#include <model/positioning/RelativePositioningInParent.h>


namespace vc {
	IngameHotbarRenderer::IngameHotbarRenderer(TextureArray arr, Renderer2D& renderer2D, egui::MasterRenderer& eguiRenderer, egui::EGuiContext& ctx) :
			blockTextureArray(arr),
			hotbarTextureSheet(vc::getApplicationFolder().append("\\textures\\gui\\hotbar.png")),
			eguiRenderer(eguiRenderer),
			ctx(ctx),
			renderer2D(renderer2D) {

		// set label for selected element
		this->selectedElemLabel = std::shared_ptr<egui::Label>(new egui::Label());
		this->selectedElemPositioning = std::shared_ptr<egui::RelativePositioningOnScreen>(new egui::RelativePositioningOnScreen(0, 0));

		// set dimensions and positioning
		selectedElemLabel->setPreferredWidth(0, egui::RelativityMode::NULLVALUE);
		selectedElemLabel->setPreferredHeight(0, egui::RelativityMode::NULLVALUE);
		selectedElemLabel->setOwnPositioning(selectedElemPositioning);





		// set label for hotbar background
		this->hotbarElement = std::shared_ptr<egui::Label>(new egui::Label());
		hotbarElement->setPreferredDimension(HOTBAR_SPRITE_WIDTH, egui::RelativityMode::ABSOLUTE_VALUE, HOTBAR_SPRITE_HEIGHT, egui::RelativityMode::ABSOLUTE_VALUE);


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
		hotbarElement->setOwnPositioning(pos);
	}

	void IngameHotbarRenderer::renderHotbar(PlayerInventory& playerInventory, int selectedSlot) {
		glActiveTexture(GL_TEXTURE0);

		/*
		 * Render Background of hotbar.
		 */
		glBindTexture(GL_TEXTURE_2D, hotbarTextureSheet.getTexId());
		renderer2D.render(
			this->hotbarElement,
			true,
			{0, 0},
			{float(HOTBAR_SPRITE_WIDTH) / hotbarTextureSheet.getWidth(), 0},
			{float(HOTBAR_SPRITE_WIDTH) / hotbarTextureSheet.getWidth(), float(HOTBAR_SPRITE_HEIGHT) / hotbarTextureSheet.getHeight()},
			{0, float(HOTBAR_SPRITE_HEIGHT) / hotbarTextureSheet.getHeight()}
		);

		/*
		 * Render items in hotbar.
		 */
		double const imgWidth = HOTBAR_SPRITE_WIDTH;
		double const imgHeight = HOTBAR_SPRITE_HEIGHT;
		double const elemWidth = hotbarElement->getComputedWidth();
		double const elemHeight = hotbarElement->getComputedHeight();
		double const elemMarginX = hotbarElement->getAbsXMargin();
		double const elemMarginY = hotbarElement->getAbsYMargin();

		double const imgBorderWidth = (HOTBAR_SPRITE_BORDER / imgWidth) * elemWidth;
		double const imgBorderHeight = (HOTBAR_SPRITE_BORDER / imgHeight) * elemHeight;

		double const slotBorderWidth = (HOTBAR_BOX_BORDER / imgWidth) * elemWidth;
		double const slotBorderHeight = (HOTBAR_BOX_BORDER / imgHeight) * elemHeight;

		double const slotWidth = (HOTBAR_BOX_DIMENSIONS / imgWidth) * elemWidth;
		double const slotHeight = (HOTBAR_BOX_DIMENSIONS / imgHeight) * elemHeight;

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
		

		/*
		 * Render pointer for selected element.
		 */
		{
			// set positioning
			int horizontalSlotBorderAmount = 2 * selectedSlot;
			double absXMargin = elemMarginX + imgBorderWidth + horizontalSlotBorderAmount * slotBorderWidth + selectedSlot * slotWidth;
			double absYMargin = elemMarginY + imgBorderHeight + slotBorderHeight;

			double width = slotWidth + 2 * slotBorderWidth;
			double height = slotHeight + 2 * slotBorderHeight;

			selectedElemLabel->setPreferredDimension(width, egui::RelativityMode::RELATIVE_IN_PARENT, height, egui::RelativityMode::RELATIVE_IN_PARENT);
			selectedElemPositioning->setX(absXMargin);
			selectedElemPositioning->setY(absYMargin);
		}
		{
			// compute tex coords
			double xMargin = 0;
			double yMargin = float(ACTIVEELEM_MARGINY) / hotbarTextureSheet.getHeight();
			double width = float(ACTIVEELEM_SPRITE_DIMENSIONS) / hotbarTextureSheet.getWidth();
			double height = float(ACTIVEELEM_SPRITE_DIMENSIONS) / hotbarTextureSheet.getHeight();

			glBindTexture(GL_TEXTURE_2D, hotbarTextureSheet.getTexId());
			glDisable(GL_DEPTH_TEST);
			renderer2D.render(
				this->selectedElemLabel,
				true,
				{xMargin, yMargin},
				{xMargin + width, yMargin},
				{xMargin + width, yMargin + height},
				{xMargin, yMargin + height}
			);
			glEnable(GL_DEPTH_TEST);
		}
	}
}