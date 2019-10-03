#include "OptionsStateElement.h"
#include <model\nodes\VBox.h>

#include <model/nodeComponents/background/TexturedBackground.h>

#include "../../model/utils/FileUtils.h"

namespace egui {
	OptionsStateElement::OptionsStateElement(EGuiContext& ctx) :
			HBox() {
		float const btnHeight = 0.065f;

		std::shared_ptr<Slider::IntegerInterpolator> fovInterpolator(new Slider::IntegerInterpolator(0, 100, 50));
		fovSlider = std::shared_ptr<Slider>(new Slider(fovInterpolator, "FOV"));
		renderDistanceSlider = std::shared_ptr<Button>(new Button("Render distance"));
		maxFramerateSlider = std::shared_ptr<Button>(new Button("Max. Framerate"));
		useVSyncBtn = std::shared_ptr<Button>(new Button("Use VSync"));
		
		fovSlider->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		renderDistanceSlider->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		maxFramerateSlider->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		useVSyncBtn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);


		std::shared_ptr<VBox> col1(new VBox({fovSlider, renderDistanceSlider, maxFramerateSlider, useVSyncBtn}));
		std::shared_ptr<VBox> col2(new VBox());
		col1->setPreferredDimension(0.5f, RelativityMode::RELATIVE_IN_PARENT, 1, RelativityMode::RELATIVE_IN_PARENT);
		col2->setPreferredDimension(0.5f, RelativityMode::RELATIVE_IN_PARENT, 1, RelativityMode::RELATIVE_IN_PARENT);
		HBox::addChildElement(col1);
		HBox::addChildElement(col2);

		std::shared_ptr<Image> texture = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		HBox::setBackground(texturedBackground);
	}
}
