#pragma once

#include <model/nodes/UnorganizedParentElement.h>
#include <model/nodes/Button.h>
#include <model/nodes/Label.h>


namespace egui {
	class MainMenuElement : public UnorganizedParentElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<Button> startGameBtn;
			std::shared_ptr<Button> optionsBtn;
			std::shared_ptr<Button> exitGameBtn;
			std::shared_ptr<Label> versionLabel;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MainMenuElement(EGuiContext& ctx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setOnStartGame(FunctionWrapper<ActionEvent> eventHandler);
			void setOnOpenOptions(FunctionWrapper<ActionEvent> eventHandler);
			void setOnExitGame(FunctionWrapper<ActionEvent> eventHandler);
	};
}