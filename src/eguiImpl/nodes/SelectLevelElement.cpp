#include "SelectLevelElement.h"
#include <model\nodes\VBox.h>
#include <model/nodeComponents/background/TexturedBackground.h>
#include <model/positioning/RelativePositioningOnScreen.h>
#include "../../model/utils/FileUtils.h"


namespace egui {
	SelectLevelElement::SelectLevelElement(std::vector<std::shared_ptr<vc::LevelMetadata>>& levelMetadatas, selectLevelFunct selectFunction, deleteLevelFunct deleteFunction, createLevelFunct createFunction) :
			levelMetadatas(levelMetadatas),
			selectFunction(selectFunction),
			deleteFunction(deleteFunction),
			createFunction(createFunction) {

		float const btnHeight = 0.065f;
		float const verticalSpaceBetweenButtons = 0.02f;
		float const selectBtnWidth = 0.85f;
		float const spaceBetweenButtons = 0.05f;
		float const deleteBtnWidth = 0.1f;

		std::shared_ptr<VBox> row(new VBox());
		row->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, 1 - btnHeight, RelativityMode::RELATIVE_IN_PARENT);
		row->setSpaceBetweenElements(verticalSpaceBetweenButtons, RelativityMode::RELATIVE_IN_PARENT);
		for(int counter = 0; counter < NUMBER_OF_BTNS; counter++) {	
			// create elements
			selectButtons[counter] = std::shared_ptr<Button>(new Button(""));
			deleteButtons[counter] = std::shared_ptr<Button>(new Button("X"));
			buttonBoxes[counter] = std::shared_ptr<HBox>(new HBox());

			// set dimensions
			buttonBoxes[counter]->setPreferredHeight(btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
			selectButtons[counter]->setPreferredWidth(selectBtnWidth, RelativityMode::RELATIVE_IN_PARENT);
			deleteButtons[counter]->setPreferredWidth(deleteBtnWidth, RelativityMode::RELATIVE_IN_PARENT);
			buttonBoxes[counter]->setSpaceBetweenElements(spaceBetweenButtons, RelativityMode::RELATIVE_IN_PARENT);

			buttonBoxes[counter]->setConsiderElemStretchFlag(false);

			// add buttonBox to row
			row->addChildElement(buttonBoxes[counter]);

			// set event handlers
			selectButtons[counter]->getActionEventManager().addEventHandler({[this, counter](ActionEvent&) {
				this->selectFunction(this->levelMetadatas[counter]);
				rebuildElem();
			}});
			deleteButtons[counter]->getActionEventManager().addEventHandler({ [this, counter](ActionEvent&) {
				this->deleteFunction(this->levelMetadatas[counter]);
				rebuildElem();
			}});
		}

		this->rebuildElem();

		float const controlButtonBoxSpace = 0.065f;
		backBtn = std::shared_ptr<Button>(new Button("Back"));
		backBtn->setPreferredWidth(0.5f - (controlButtonBoxSpace / 2), RelativityMode::RELATIVE_IN_PARENT);
		createNewGameScoreBtn = std::shared_ptr<Button>(new Button("Create new level"));
		createNewGameScoreBtn->setPreferredWidth(0.5f - (controlButtonBoxSpace / 2), RelativityMode::RELATIVE_IN_PARENT);
		createNewGameScoreBtn->getActionEventManager().addEventHandler({[this](ActionEvent&) {
			this->createFunction();
			rebuildElem();
		}});
		std::shared_ptr<HBox> controlButtonBox(new HBox(controlButtonBoxSpace, RelativityMode::RELATIVE_IN_PARENT, { backBtn, createNewGameScoreBtn }));
		controlButtonBox->setPreferredHeight(btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		controlButtonBox->setConsiderElemStretchFlag(false);
		
		std::shared_ptr<VBox> table(new VBox({row, controlButtonBox}));
		table->setOwnPositioning(std::shared_ptr<Positioning>(new RelativePositioningOnScreen(0.1, 0.05)));
		table->setPreferredDimension(0.8, RelativityMode::RELATIVE_IN_PARENT, 0.9, RelativityMode::RELATIVE_IN_PARENT);
		UnorganizedParentElement::addChildElement(table);

		// set background
		std::shared_ptr<Image> texture = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		UnorganizedParentElement::setBackground(texturedBackground);
	}

	void SelectLevelElement::rebuildElem() {
		float const btnHeight = 0.065f;


		// hide all buttons
		for(int counter = 0; counter < NUMBER_OF_BTNS; counter++) {
			if(buttonBoxes[counter]->containsChildElement(deleteButtons[counter])) {
				buttonBoxes[counter]->removeChildElement(deleteButtons[counter]);
			}
			if(buttonBoxes[counter]->containsChildElement(selectButtons[counter])) {
				buttonBoxes[counter]->removeChildElement(selectButtons[counter]);
			}
		}

		for(int counter = 0; counter < NUMBER_OF_BTNS && counter < levelMetadatas.size(); counter++) {
			selectButtons[counter]->setText(levelMetadatas[counter]->getLevelName());

			buttonBoxes[counter]->addChildElement(selectButtons[counter]);
			buttonBoxes[counter]->addChildElement(deleteButtons[counter]);
		}
	}

	void egui::SelectLevelElement::setOnBackBtn(FunctionWrapper<ActionEvent> eventHandler) {
		backBtn->getActionEventManager().addEventHandler(eventHandler);
	}



}