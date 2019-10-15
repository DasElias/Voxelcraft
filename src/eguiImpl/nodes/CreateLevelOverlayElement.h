#pragma once
#include <model/nodes/UnorganizedParentElement.h>
#include <model/nodes/Button.h>
#include <model/nodes/InputField.h>

#include <model/events/ActionEvent.h>

namespace egui {
	typedef std::function<void(std::string, std::string)> createLevelFunct;

	class CreateLevelOverlayElement : public UnorganizedParentElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<Button> submitBtn;
			std::shared_ptr<Button> exitBtn;
			std::shared_ptr<InputField> levelNameInputField;
			std::shared_ptr<InputField> seedInputField;

			createLevelFunct createFunction;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateLevelOverlayElement(createLevelFunct createFunction);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void show();
			void hide();
	};
}