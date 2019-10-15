#include "SelectLevelElement.h"
#include <model\nodes\VBox.h>
#include <model/nodeComponents/background/TexturedBackground.h>
#include <model/positioning/RelativePositioningOnScreen.h>
#include "../../model/utils/FileUtils.h"
#include <model/positioning/CenterAllInParentWrapper.h>
#include <iostream>

namespace egui {
	SelectLevelElement::SelectLevelElement(std::vector<std::shared_ptr<vc::LevelMetadata>>& levelMetadatas, selectLevelFunct selectFunction, deleteLevelFunct deleteFunction, createLevelFunct createFunction, exportFunct exportFunction, importFuct importFunction) :
			levelMetadatas(levelMetadatas),
			selectFunction(selectFunction),
			deleteFunction(deleteFunction),
			exportFunction(exportFunction),
			importFunction(importFunction) {



		float const btnHeight = 0.065f;
		float const verticalSpaceBetweenButtons = 0.02f;
		float const selectBtnWidth = 0.75f;
		float const deleteBtnWidth = 0.1f;
		float const exportBtnWidth = 0.1f;
		float const spaceBetweenButtons = (1 - selectBtnWidth - deleteBtnWidth - exportBtnWidth) / 3;
		float const verticalSpaceBetweenControlButtons = 0.03f;

		std::shared_ptr<VBox> row(new VBox());
		row->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, 1 - 2 * btnHeight - 2 * verticalSpaceBetweenButtons, RelativityMode::RELATIVE_IN_PARENT);
		row->setSpaceBetweenElements(verticalSpaceBetweenButtons, RelativityMode::RELATIVE_IN_PARENT);
		for(int counter = 0; counter < NUMBER_OF_BTNS; counter++) {	
			// create elements
			selectButtons[counter] = std::shared_ptr<Button>(new Button(""));
			deleteButtons[counter] = std::shared_ptr<Button>(new Button("Delete"));
			exportButtons[counter] = std::shared_ptr<Button>(new Button("Export"));
			buttonBoxes[counter] = std::shared_ptr<HBox>(new HBox());

			// set dimensions
			buttonBoxes[counter]->setPreferredHeight(btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
			selectButtons[counter]->setPreferredWidth(selectBtnWidth, RelativityMode::RELATIVE_IN_PARENT);
			deleteButtons[counter]->setPreferredWidth(deleteBtnWidth, RelativityMode::RELATIVE_IN_PARENT);
			exportButtons[counter]->setPreferredWidth(exportBtnWidth, RelativityMode::RELATIVE_IN_PARENT);
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
			exportButtons[counter]->getActionEventManager().addEventHandler({ [this, counter](ActionEvent&) {
				this->exportFunction(this->levelMetadatas[counter]);
				rebuildElem();
			}});
		}


		float const controlButtonBoxSpace = 0.065f;
		backBtn = std::shared_ptr<Button>(new Button("Back"));
		backBtn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);

		createNewGameScoreBtn = std::shared_ptr<Button>(new Button("Create new level"));
		createNewGameScoreBtn->setPreferredWidth(0.5f - (controlButtonBoxSpace / 2), RelativityMode::RELATIVE_IN_PARENT);
		createNewGameScoreBtn->getActionEventManager().addEventHandler({[this](ActionEvent&) {
			this->createLevelOverlayElement->show();
			//rebuildElem();
		}});

		importBtn = std::shared_ptr<Button>(new Button("Import level"));
		importBtn->setPreferredWidth(0.5f - (controlButtonBoxSpace / 2), RelativityMode::RELATIVE_IN_PARENT);
		importBtn->getActionEventManager().addEventHandler({[this](ActionEvent&) {
			this->importFunction();
			rebuildElem();
		}});

		std::shared_ptr<HBox> controlButtonBox(new HBox(controlButtonBoxSpace, RelativityMode::RELATIVE_IN_PARENT, { importBtn, createNewGameScoreBtn }));
		controlButtonBox->setPreferredHeight(btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		controlButtonBox->setConsiderElemStretchFlag(false);
		
		std::shared_ptr<VBox> table(new VBox({row, controlButtonBox, backBtn}));
		table->setOwnPositioning(std::shared_ptr<Positioning>(new RelativePositioningOnScreen(0.1, 0.05)));
		table->setPreferredDimension(0.8, RelativityMode::RELATIVE_IN_PARENT, 0.9, RelativityMode::RELATIVE_IN_PARENT);
		table->setSpaceBetweenElements(verticalSpaceBetweenControlButtons, RelativityMode::RELATIVE_IN_PARENT);
		UnorganizedParentElement::addChildElement(table);

		// button, which is shown, when no level exists
		noLevelFoundWarning = std::shared_ptr<Label>(new Label("No world was found! Create one using the \"Create world\"-Button!", 23, false, Text::HorizontalAlignment::CENTER, Text::VerticalAlignment::MIDDLE, { 1, 1, 1 }));
		noLevelFoundWarning->setOwnPositioning(std::shared_ptr<Positioning>(new CenterAllInParentWrapper()));
		UnorganizedParentElement::addChildElement(noLevelFoundWarning);

		// rebuild elem to synchronize the displayed buttons with levelMetadatas
		this->rebuildElem();

		// set background
		std::shared_ptr<Image> texture = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		UnorganizedParentElement::setBackground(texturedBackground);

		this->createLevelOverlayElement = std::shared_ptr<CreateLevelOverlayElement>(new CreateLevelOverlayElement(createFunction));
		this->createLevelOverlayElement->hide();
		UnorganizedParentElement::addChildElement(createLevelOverlayElement);
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
			if(buttonBoxes[counter]->containsChildElement(exportButtons[counter])) {
				buttonBoxes[counter]->removeChildElement(exportButtons[counter]);
			}
		}

		// show the needed buttons
		for(int counter = 0; counter < NUMBER_OF_BTNS && counter < levelMetadatas.size(); counter++) {
			// set text of the btn to the level name
			selectButtons[counter]->setText(levelMetadatas[counter]->getLevelName());

			buttonBoxes[counter]->addChildElement(selectButtons[counter]);
			buttonBoxes[counter]->addChildElement(deleteButtons[counter]);
			buttonBoxes[counter]->addChildElement(exportButtons[counter]);
		}

		// if no world was found, display a warning 
		noLevelFoundWarning->setVisible(levelMetadatas.empty());
		std::cout << noLevelFoundWarning->isVisible();
	}

	void egui::SelectLevelElement::setOnBackBtn(FunctionWrapper<ActionEvent> eventHandler) {
		backBtn->getActionEventManager().addEventHandler(eventHandler);
	}



}