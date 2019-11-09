#include "CreateLevelOverlayElement.h"

#include <model/nodeComponents/background/TexturedBackground.h>
#include <model/nodeComponents/background/ColoredBackground.h>
#include <model/positioning/CenterAllInParentWrapper.h>
#include <input/IOHandler.h>
#include <model/nodes/VBox.h>
#include <model/nodes/HBox.h>
#include "../../model/utils/FileUtils.h"
#include <model/nodes/Label.h>
#include <model\positioning\RelativePositioningOnScreen.h>
#include <model\positioning\RelativePositioningInParent.h>


namespace egui {
	CreateLevelOverlayElement::CreateLevelOverlayElement(createLevelFunct createFunction) :
			createFunction(createFunction) {

		float const btnHeight = 0.065f;


		// add key handler for ESCAPE
		getInputHandler().getKeyEventManager().addEventHandler({[this](KeyEvent& evt) {
			if(evt.getKey() == KEY_ESCAPE) {
				this->hide();
			}
		}});

		setConsumeAllMouseEvents(true);

		// set background
		UnorganizedParentElement::setBackground(std::shared_ptr<Background>(
			new ColoredBackground(
				Color(0, 0, 0, 0.8f)
			)	
		));

		// title
		std::shared_ptr<Label> title(new Label("Create new world", 0.65f, true, Text::HorizontalAlignment::CENTER, Text::VerticalAlignment::MIDDLE, Color(1, 1, 1)));
		title->setPreferredHeight({0.15f, RelativityMode::RELATIVE_IN_PARENT});

		float const verticalSpaceBetweenButtons = 0.03f;

		// create submit button
		this->submitBtn = std::shared_ptr<Button>(new Button("Create world"));
		this->submitBtn->setPreferredDimension({0.5f - (verticalSpaceBetweenButtons / 2), RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		this->submitBtn->getActionEventManager().addEventHandler({ [this](ActionEvent&) {
			this->createFunction(levelNameInputField->getText(), seedInputField->getText());
		}});

		// create exit button
		this->exitBtn = std::shared_ptr<Button>(new Button("Cancel"));
		this->exitBtn->setPreferredDimension({0.5f - (verticalSpaceBetweenButtons / 2), RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		this->exitBtn->getActionEventManager().addEventHandler({ [this](ActionEvent&) {
			this->hide();
		}});

		// create button bar
		std::shared_ptr<HBox> buttonBar(new HBox{exitBtn, submitBtn});
		buttonBar->setPreferredHeight({btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		buttonBar->setSpaceBetweenElements({0.03f, RelativityMode::RELATIVE_IN_PARENT});
		buttonBar->setConsiderElemStretchFlag(false);

		float const labelFontSize = 0.7f;
		float const labelHeight = 0.3f;
		float const inputBoxHeight = 0.2f;

		// create level name input
		this->levelNameInputField = std::shared_ptr<InputField>(new InputField("Hallo"));
		this->levelNameInputField->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		std::shared_ptr<Label> levelNameInputLabel(new Label("World name", labelFontSize, true, Text::HorizontalAlignment::LEFT, Text::VerticalAlignment::MIDDLE, Color(1, 1, 1)));
		levelNameInputLabel->setPreferredHeight({labelHeight, RelativityMode::RELATIVE_IN_PARENT});
		std::shared_ptr<VBox> levelNameInputBox(new VBox({levelNameInputLabel, levelNameInputField}));
		levelNameInputBox->setPreferredHeight({inputBoxHeight, RelativityMode::RELATIVE_IN_PARENT});

		// create seed input
		this->seedInputField = std::shared_ptr<InputField>(new InputField("Hallo"));
		this->seedInputField->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		std::shared_ptr<Label> seedInputLabel(new Label("Seed (for a random generated seed, leave this field empty)", labelFontSize, true, Text::HorizontalAlignment::LEFT, Text::VerticalAlignment::MIDDLE, Color(1, 1, 1)));
		seedInputLabel->setPreferredHeight({labelHeight, RelativityMode::RELATIVE_IN_PARENT});
		std::shared_ptr<VBox> seedInputBox(new VBox({seedInputLabel, seedInputField}));
		seedInputBox->setPreferredHeight({inputBoxHeight, RelativityMode::RELATIVE_IN_PARENT});

		std::shared_ptr<VBox> intendedContent(new VBox({title, levelNameInputBox, seedInputBox}));
		intendedContent->setPreferredWidth({0.8f, RelativityMode::RELATIVE_IN_PARENT});
		intendedContent->setOwnPositioning(std::make_shared<RelativePositioningInParent>(0.1f));

		std::shared_ptr<UnorganizedParentElement> elemContent(new UnorganizedParentElement({intendedContent}));
		static std::shared_ptr<Image> img = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\createLevelBackground.png"), true);
		elemContent->setBackground(std::shared_ptr<Background>(new TexturedBackground(img)));
		elemContent->setRadius(0.02f, true);
		elemContent->setPreferredDimension({0.8f, RelativityMode::RELATIVE_IN_PARENT}, {0.8f, RelativityMode::RELATIVE_IN_PARENT});
		elemContent->setOwnPositioning(std::shared_ptr<Positioning>(new CenterAllInParentWrapper()));
		elemContent->setBorder(std::make_shared<Border>(Color(), BorderType::SOLID));
		UnorganizedParentElement::addChildElement(elemContent);

		// set positioning of button bar
		buttonBar->setOwnPositioning(std::make_shared<RelativePositioningInParent>(0.1f, 1 - btnHeight - 0.1f));
		buttonBar->setPreferredWidth({0.8f, RelativityMode::RELATIVE_IN_PARENT});
		elemContent->addChildElement(buttonBar);
	}
	void CreateLevelOverlayElement::show() {
		setVisible(true);
	}
	void CreateLevelOverlayElement::hide() {
		setVisible(false);
	}
}
