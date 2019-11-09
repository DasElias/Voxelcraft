#include "MainMenuElement.h"
#include <model/nodeComponents/background/TexturedBackground.h>
#include <model\nodes\HBox.h>
#include <model\nodes\VBox.h>
#include <model\positioning\CenterXInParentWrapper.h>
#include <model\positioning\RelativePositioningInParent.h>
#include "../../model/utils/FileUtils.h"


namespace egui {
	MainMenuElement::MainMenuElement(EGuiContext& ctx) {
		float btnHeight = 0.065f;
		startGameBtn = std::shared_ptr<Button>(new Button("Start game"));
		startGameBtn->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});

		optionsBtn = std::shared_ptr<Button>(new Button("Options"));
		optionsBtn->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});

		exitGameBtn = std::shared_ptr<Button>(new Button("Exit game"));
		exitGameBtn->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});


		std::shared_ptr<VBox> buttonPanel(new VBox({0.03f, RelativityMode::RELATIVE_IN_PARENT}, {startGameBtn, optionsBtn, exitGameBtn}));
		buttonPanel->setPreferredWidth({0.3f, RelativityMode::RELATIVE_IN_PARENT});
		buttonPanel->setMinWidth({300, egui::RelativityMode::ABSOLUTE_VALUE});
		std::shared_ptr<CenterXInParentWrapper> rootPositioning(new CenterXInParentWrapper(std::shared_ptr<Positioning>(new RelativePositioningInParent(0, 0.6f))));
		buttonPanel->setOwnPositioning(rootPositioning);

		versionLabel = std::shared_ptr<Label>(new Label(" by DasElias", 20, false, Text::HorizontalAlignment::LEFT, Text::VerticalAlignment::BOTTOM, {1, 1, 1}));
		versionLabel->getTextComponent()->setForceOneLine(true);

		// set root elements
		UnorganizedParentElement::addChildElement(buttonPanel);
		UnorganizedParentElement::addChildElement(versionLabel);

		// set background
		std::shared_ptr<Image> texture = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		UnorganizedParentElement::setBackground(texturedBackground);
	}

	void MainMenuElement::setOnStartGame(FunctionWrapper<ActionEvent> eventHandler) {
		startGameBtn->getActionEventManager().addEventHandler(eventHandler);
	}

	
	void MainMenuElement::setOnOpenOptions(FunctionWrapper<ActionEvent> eventHandler) {
		optionsBtn->getActionEventManager().addEventHandler(eventHandler);
	}

	void MainMenuElement::setOnExitGame(FunctionWrapper<ActionEvent> eventHandler) {
		exitGameBtn->getActionEventManager().addEventHandler(eventHandler);
	}

}
