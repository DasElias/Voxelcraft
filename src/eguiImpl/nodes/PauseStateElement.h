#pragma once

#include <model/nodes/UnorganizedParentElement.h>
#include <model/nodes/Button.h>
#include <model/nodes/Label.h>

namespace egui {
	class PauseStateElement : public UnorganizedParentElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<Button> backToGameBtn;
			std::shared_ptr<Button> optionsBtn;
			std::shared_ptr<Button> mainMenuBtn;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PauseStateElement(EGuiContext& ctx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setOnBackToGame(FunctionWrapper<ActionEvent> handler);
			void setOnOptionsBtn(FunctionWrapper<ActionEvent> handler);
			void setOnMainMenuBtn(FunctionWrapper<ActionEvent> handler);
	};
}