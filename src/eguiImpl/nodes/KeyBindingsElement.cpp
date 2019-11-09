#include "KeyBindingsElement.h"
#include <model\nodes\VBox.h>

#include <model/nodeComponents/background/TexturedBackground.h>
#include <model/nodeComponents/background/ColoredBackground.h>
#include <model/positioning/RelativePositioningOnScreen.h>
#include "../../model/utils/FileUtils.h"
#include "KeyBindingsElement.h"

namespace egui {
	KeyBindingsElement::KeyBindingsElement(EGuiContext& ctx) {
		float const btnHeight = 0.065f;
		float const verticalSpaceBetweenButtons = 0.02f;
		float const horizontalSpaceBetweenRows = 0.05f;

		std::shared_ptr<KeySelectButton> moveForward(new KeySelectButton("MOVE_FORWARD", "Move forward"));
		std::shared_ptr<KeySelectButton> moveBackward(new KeySelectButton("MOVE_BACKWARD", "Move backward"));
		std::shared_ptr<KeySelectButton> moveLeft(new KeySelectButton("MOVE_LEFT", "Move left"));
		std::shared_ptr<KeySelectButton> moveRight(new KeySelectButton("MOVE_RIGHT", "Move right"));
		std::shared_ptr<KeySelectButton> openInventory(new KeySelectButton("OPEN_INVENTORY", "Open inventory"));

		backButton = std::shared_ptr<Button>(new Button("Back"));
		backButton->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});

		// create grid structure
		std::shared_ptr<VBox> col1(new VBox({moveForward, moveBackward, moveLeft, moveRight, openInventory}));
		std::shared_ptr<VBox> col2(new VBox());
		col1->setPreferredDimension({0.5f - (horizontalSpaceBetweenRows / 2), RelativityMode::RELATIVE_IN_PARENT}, {1, RelativityMode::RELATIVE_IN_PARENT});
		col2->setPreferredDimension({0.5f - (horizontalSpaceBetweenRows / 2), RelativityMode::RELATIVE_IN_PARENT}, {1, RelativityMode::RELATIVE_IN_PARENT});
		col1->setSpaceBetweenElements({verticalSpaceBetweenButtons, RelativityMode::RELATIVE_ON_SCREEN});
		col2->setSpaceBetweenElements({verticalSpaceBetweenButtons, RelativityMode::RELATIVE_ON_SCREEN});

		std::shared_ptr<HBox> row1(new HBox({col1, col2}));
		row1->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {1 - btnHeight, RelativityMode::RELATIVE_IN_PARENT});
		row1->setSpaceBetweenElements({horizontalSpaceBetweenRows, RelativityMode::RELATIVE_IN_PARENT});

		std::shared_ptr<VBox> table(new VBox({row1, backButton}));
		table->setOwnPositioning(std::shared_ptr<Positioning>(new RelativePositioningOnScreen(0.1, 0.05)));
		table->setPreferredDimension({0.8, RelativityMode::RELATIVE_IN_PARENT}, {0.9, RelativityMode::RELATIVE_IN_PARENT});
		UnorganizedParentElement::addChildElement(table);

		// add KeySelectButtons for slots
		for(int i = 1; i <= 9; i++) {
			col2->addChildElement(std::shared_ptr<KeySelectButton>(new KeySelectButton(
				"SLOT" + std::to_string(i),
				"Fast access " + std::to_string(i)
			)));
		}

		// set preferred dimensions
		for(auto& a : col1->getChildrenElements()) {
			a->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		}
		for(auto& a : col2->getChildrenElements()) {
			a->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {btnHeight, RelativityMode::RELATIVE_ON_SCREEN});
		}

		// set background
		std::shared_ptr<Image> texture = Image::loadTexture(vc::getApplicationFolder().append("\\textures\\gui\\mainMenuBackground.png"));
		std::shared_ptr<Background> texturedBackground(new TexturedBackground(texture));
		UnorganizedParentElement::setBackground(texturedBackground);
	}
	void KeyBindingsElement::setOnBackButtonAction(FunctionWrapper<ActionEvent> backButtonAction) {
		backButton->getActionEventManager().addEventHandler(backButtonAction);
	}
}
