#include "PauseStateElement.h"

#include <model\nodes\HBox.h>
#include <model\nodes\VBox.h>
#include <model\positioning\CenterXInParentWrapper.h>
#include <model\positioning\RelativePositioningInParent.h>
#include "../../model/utils/FileUtils.h"



namespace egui {
	PauseStateElement::PauseStateElement(EGuiContext& ctx) {
		float const btnHeight = 0.065f;

		backToGameBtn = std::shared_ptr<Button>(new Button("Back to Game"));
		backToGameBtn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		optionsBtn = std::shared_ptr<Button>(new Button("Options"));
		optionsBtn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);
		mainMenuBtn = std::shared_ptr<Button>(new Button("Main Menu"));
		mainMenuBtn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, btnHeight, RelativityMode::RELATIVE_ON_SCREEN);

		std::shared_ptr<VBox> box(new VBox(0.03f, RelativityMode::RELATIVE_IN_PARENT, {backToGameBtn, optionsBtn, mainMenuBtn}));
		box->setPreferredWidth(0.3, RelativityMode::RELATIVE_IN_PARENT);
		box->setMinWidth(300, egui::RelativityMode::ABSOLUTE_VALUE);
		std::shared_ptr<CenterXInParentWrapper> rootPositioning(new CenterXInParentWrapper(std::shared_ptr<Positioning>(new RelativePositioningInParent(0, 0.3f))));
		box->setOwnPositioning(rootPositioning);

		UnorganizedParentElement::addChildElement(box);
	}

	void PauseStateElement::setOnBackToGame(FunctionWrapper<ActionEvent> handler) {
		backToGameBtn->getActionEventManager().addEventHandler(handler);
	}

	void PauseStateElement::setOnOptionsBtn(FunctionWrapper<ActionEvent> handler) {
		optionsBtn->getActionEventManager().addEventHandler(handler);
	}

	void PauseStateElement::setOnMainMenuBtn(FunctionWrapper<ActionEvent> handler) {
		mainMenuBtn->getActionEventManager().addEventHandler(handler);
	}
}
