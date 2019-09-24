#include "CreativeInventoryGUI.h"
#include <model\nodes\UnorganizedParentElement.h>
#include <model\nodes\VBox.h>
#include <model/nodeComponents/background/TexturedBackground.h>

#include <model/nodeComponents/background/ColoredBackground.h>
#include "../../model/utils/FileUtils.h"

using namespace egui;

namespace vc {
	CreativeInventoryGUI::CreativeInventoryGUI() {
		std::shared_ptr<VBox> parent(new VBox());

		static std::shared_ptr<Image> img = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\creativeInventory.png"));
		parent->setBackground(std::shared_ptr<Background>(
			new TexturedBackground(
				img
			)
		));


		parent->setPreferredDimension(0.7f, true, 0.8f, true);
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