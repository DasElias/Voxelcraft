#pragma once

#include <themes/AbstractTheme.h>

namespace egui {
	class VoxelcraftTheme : public AbstractTheme {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static float const BTN_HEIGHT;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			VoxelcraftTheme(EGuiContext& ctx);


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::function<void(Node* const)> button_keySelectButton_apply();
			std::function<void(Node* const)> inputField_apply();
	};
}