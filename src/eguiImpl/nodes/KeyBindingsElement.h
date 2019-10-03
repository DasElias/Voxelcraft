#pragma once

#include <model/nodes/VBox.h>
#include <model/nodes/UnorganizedParentElement.h>
#include <model/nodes/HBox.h>
#include <model/nodes/KeySelectButton.h>
#include <model\nodes\Button.h>

#include <input/KeyAssignments.h>



namespace egui {
	class KeyBindingsElement : public UnorganizedParentElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<Button> backButton;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			KeyBindingsElement(EGuiContext& ctx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setOnBackButtonAction(FunctionWrapper<ActionEvent> backButtonAction);
		
	};
}