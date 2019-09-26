
#include "CreativeInventoryGUI.h"
#include <model\nodes\UnorganizedParentElement.h>
#include <model\nodes\VBox.h>
#include <model\nodes\AspectRatioElement.h>

#include <model/nodeComponents/background/TexturedBackground.h>

#include <model/nodeComponents/background/ColoredBackground.h>
#include "../../model/utils/FileUtils.h"
#include <model/positioning/CenterAllInParentWrapper.h>
#include <model/positioning/RelativePositioningInParent.h>


using namespace egui;

namespace vc {
	CreativeInventoryGUI::CreativeInventoryGUI() {
		static std::shared_ptr<Image> img = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\creativeInventory.png"));
		float imgWidth = img->getWidth();
		float imgHeight = img->getHeight();
		float ar = imgWidth / imgHeight;
		std::shared_ptr<AspectRatioElement> parent(new AspectRatioElement(ar));
		
		parent->setBackground(std::shared_ptr<Background>(
			new TexturedBackground(
				img
			)
		));


		parent->setMaxDimension(0.8f, true, 0.8f, true);
		parent->setOwnPositioning(std::shared_ptr<Positioning>(new egui::CenterAllInParentWrapper(std::shared_ptr<Positioning>(new RelativePositioningInParent(0, 0)))));
		inventoryGUI.setRootElement(parent);
	}

	void CreativeInventoryGUI::render(Player& p, egui::MasterRenderer& eguiRenderer, const TextureArray& texArray) {
		eguiRenderer.beginFrame();
		inventoryGUI.render(eguiRenderer);
		eguiRenderer.endFrame();
	}

	void CreativeInventoryGUI::click(Player& p, float percentX, float percentY) {
	}

}