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
		createGameScoreBtn = std::shared_ptr<Button>(new Button("Neues Spiel erstellen"));
		createGameScoreBtn->setPreferredDimension(1, true, btnHeight, true);

		loadGameScoreBtn = std::shared_ptr<Button>(new Button("Spielstand laden"));
		loadGameScoreBtn->setPreferredDimension(1, true, btnHeight, true);



		// create HBox with third and fourth button
		float const hBoxSpace = 0.065f;
		optionsBtn = std::shared_ptr<Button>(new Button("Optionen"));
		optionsBtn->setPreferredWidth(0.5f - (hBoxSpace / 2), true);

		exitGameBtn = std::shared_ptr<Button>(new Button("Spiel beenden"));
		exitGameBtn->setPreferredWidth(0.5f - (hBoxSpace / 2), true);

		std::shared_ptr<HBox> hBox(new HBox(hBoxSpace, true, {optionsBtn, exitGameBtn}));
		hBox->setPreferredHeight(btnHeight, true);
		hBox->setConsiderElemStretchFlag(false);

		std::shared_ptr<VBox> buttonPanel(new VBox(0.03f, true, {createGameScoreBtn, loadGameScoreBtn, hBox}));
		buttonPanel->setPreferredWidth(0.3f, true);
		buttonPanel->setMinWidth(300, false);
		std::shared_ptr<CenterXInParentWrapper> rootPositioning(new CenterXInParentWrapper(std::shared_ptr<Positioning>(new RelativePositioningInParent(0, 0.6f))));
		buttonPanel->setOwnPositioning(rootPositioning);

		versionLabel = std::shared_ptr<Label>(new Label(" by DasElias", 20, false, Text::HorizontalAlignment::LEFT, Text::VerticalAlignment::BOTTOM, {1, 1, 1}));
		versionLabel->getTextComponent()->setForceOneLine(true);

		// set root elements
		UnorganizedParentElement::addChildElement(buttonPanel);
		UnorganizedParentElement::addChildElement(versionLabel);

		// set background
		std::shared_ptr<Image> texture = Image::loadTexture(ctx, vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		UnorganizedParentElement::setBackground(texturedBackground);
	}

	void MainMenuElement::setOnCreateGameScore(FunctionWrapper<ActionEvent> eventHandler) {
		createGameScoreBtn->getActionEventManager().addEventHandler(eventHandler);
	}

	void MainMenuElement::setOnLoadGameScore(FunctionWrapper<ActionEvent> eventHandler) {
		loadGameScoreBtn->getActionEventManager().addEventHandler(eventHandler);
	}

	void MainMenuElement::setOnOpenOptions(FunctionWrapper<ActionEvent> eventHandler) {
		optionsBtn->getActionEventManager().addEventHandler(eventHandler);
	}

	void MainMenuElement::setOnExitGame(FunctionWrapper<ActionEvent> eventHandler) {
		exitGameBtn->getActionEventManager().addEventHandler(eventHandler);
	}

	void MainMenuElement::setLoadButtonDisabled(bool state) {
		loadGameScoreBtn->setDisabled(state);
	}

}
