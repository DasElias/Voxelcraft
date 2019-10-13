#include "VoxelcraftTheme.h"

#include "..//..//model/utils/FileUtils.h"
#include <model\nodes\Labeled.h>
#include <model\nodes\Button.h>
#include <model\nodes\InputField.h>
#include <model/nodes/KeySelectButton.h>
#include <model\nodes\Slider.h>

#include <model/utils/LinearGradient.h>
#include <model/nodeComponents/background/ColoredBackground.h>
#include <model/nodeComponents/background/TexturedBackground.h>

#include <model/nodeComponents/background/LinearGradientBackground.h>
#include <stdexcept>

float const egui::VoxelcraftTheme::BTN_HEIGHT = 0.065f;

egui::VoxelcraftTheme::VoxelcraftTheme(EGuiContext& context) :
	AbstractTheme(context, [this]() {
		std::string applFolder = vc::getApplicationFolder();

		// LABELED
		std::shared_ptr<Font> defaultFont = Font::createFont(ctx, "CodersCrux", std::string(applFolder).append("\\resources\\fonts\\coders_crux.ttf"));
		std::shared_ptr<Font> emojiFont = Font::createFont(ctx, "EmojiFallback", std::string(applFolder).append("\\resources\\fonts\\OpenSansEmoji.ttf"));
		defaultFont->addFallbackFont(ctx, emojiFont);
		AbstractTheme::addComponent(Labeled::getClassName_static(), [&defaultFont, &emojiFont](Node* p_node) {
			Labeled* p_labeled = dynamic_cast<Labeled*>(p_node);
			if(!p_labeled) throw std::logic_error("Component is not of type Labeled!");

			p_labeled->getTextComponent()->setFont(Font::getFont("CodersCrux"));
		});

		// Button
		AbstractTheme::addComponent(Button::getClassName_static(), button_keySelectButton_apply());

		// KeySelectButton
		AbstractTheme::addComponent(KeySelectButton::getClassName_static(), button_keySelectButton_apply());

		// InputField
		AbstractTheme::addComponent(InputField::getClassName_static(), inputField_apply());

	}) {
}

std::function<void(egui::Node* const)> egui::VoxelcraftTheme::button_keySelectButton_apply() {
	return [this](Node* const p_node) {
		Labeled* p_btn = dynamic_cast<Labeled*>(p_node);
		if(!p_btn) throw std::logic_error("Component is not of type Button or KeySelectButton!");

		// set dimensions
		p_btn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, BTN_HEIGHT, RelativityMode::RELATIVE_ON_SCREEN);
		p_btn->setRadius(0.2f, true);

		p_btn->getTextComponent()->setForceOneLine(true);
		p_btn->getTextComponent()->setFontSize(0.5f, true);
		p_btn->getTextComponent()->setColor(Color(1, 1, 1));

		float const colorPoint1 = 20.0f / 256;
		float const colorPoint2 = 60.0f / 256;

		std::shared_ptr<Background> bg(new LinearGradientBackground(LinearGradient(
			Color(colorPoint1, colorPoint1, colorPoint1),
			Color(colorPoint2, colorPoint2, colorPoint2),
			0.5f, 1,
			0.5f, 0
		)));
		p_btn->setBackground(bg);

		std::shared_ptr<Border> border(new Border(Color(), SOLID));
		p_btn->setBorder(border);

		// hovered
		p_btn->getHoverEventManager().addEventHandler({ [p_btn](HoverEvent& hoverEvent) {
			if(hoverEvent.isHovered()) {
				p_btn->setStretchX(30);
				p_btn->setXTranslation(-30 / 2);
			} else {
				p_btn->setStretchX(0);
				p_btn->setXTranslation(0);
			}
		} });

		// disabled
		std::shared_ptr<Background> disabledBg(new LinearGradientBackground(LinearGradient(
			Color(colorPoint1, colorPoint1, colorPoint1, 0.5f),
			Color(colorPoint2, colorPoint2, colorPoint2, 0.5f),
			0.5f, 1,
			0.5f, 0
		)));
		p_btn->setDisabledBackground(disabledBg, RENDER_EXCLUSIVELY);

		std::shared_ptr<Border> disabledBorder(new Border(Color(0, 0, 0, 0.5f), SOLID));
		p_btn->setDisabledBorder(disabledBorder, RENDER_EXCLUSIVELY);

		p_btn->getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		p_btn->getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);
	};
}

std::function<void(egui::Node* const)> egui::VoxelcraftTheme::inputField_apply() {
	return[this](Node * const p_node) {
		InputField* p_btn = dynamic_cast<InputField*>(p_node);
		if(!p_btn) throw std::logic_error("Component is not of type InputField!");

		// set dimensions
		p_btn->setPreferredDimension(1, RelativityMode::RELATIVE_IN_PARENT, BTN_HEIGHT, RelativityMode::RELATIVE_ON_SCREEN);

		p_btn->getTextComponent()->setForceOneLine(true);
		p_btn->getTextComponent()->setFontSize(0.5f, true);
		p_btn->getTextComponent()->setColor(Color(1, 1, 1));

		std::shared_ptr<Background> bg(new ColoredBackground(Color(0, 0, 0)));
		p_btn->setBackground(bg);

		std::shared_ptr<Border> border(new Border(Color(0.627f, 0.627f, 0.627f), SOLID));
		border->setWidth(2);
		p_btn->setBorder(border);

		p_btn->getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		p_btn->getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);
	};
}



