#include "OptionsStateElement.h"
#include <model\nodes\VBox.h>

#include <model/nodeComponents/background/TexturedBackground.h>

#include "../../model/utils/FileUtils.h"

namespace egui {
	OptionsStateElement::OptionsStateElement(EGuiContext& ctx) {
		float const btnHeight = 0.065f;

		fovSlider = std::shared_ptr<Button>(new Button("FOV"));
		renderDistanceSlider = std::shared_ptr<Button>(new Button("Render distance"));
		maxFramerateSlider = std::shared_ptr<Button>(new Button("Max. Framerate"));
		useVSyncBtn = std::shared_ptr<Button>(new Button("Use VSync"));
		
		fovSlider->setPreferredDimension(1, true, btnHeight, true);
		renderDistanceSlider->setPreferredDimension(1, true, btnHeight, true);
		maxFramerateSlider->setPreferredDimension(1, true, btnHeight, true);
		useVSyncBtn->setPreferredDimension(1, true, btnHeight, true);

		std::shared_ptr<VBox> col1(new VBox({fovSlider, renderDistanceSlider, maxFramerateSlider, useVSyncBtn}));
		std::shared_ptr<VBox> col2(new VBox());
		col1->setPreferredDimension(0.5f, true, 1, true);
		col2->setPreferredDimension(0.5f, true, 1, true);
		HBox::addChildElement(col1);
		HBox::addChildElement(col2);

		std::shared_ptr<Image> texture = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		HBox::setBackground(texturedBackground);
	}
}
