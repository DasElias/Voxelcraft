#pragma once

#include <GLFW/glfw3.h>
#include "input/IOHandler.h"

namespace vc::system {

	class Lwjgl3InputHandler : public egui::InputHandler {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			GLFWwindow* window;
			egui::EventManager<egui::ScrollEvent> scrollEventManager;
			egui::EventManager<egui::MouseEvent> mouseBtnEventManager;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Lwjgl3InputHandler(GLFWwindow* window);
			~Lwjgl3InputHandler();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isKeyDown(int key) override;
			void setCursorInputMode(int cursorMode) override;
			egui::EventManager<egui::ScrollEvent>& getScrollEventManager() override;
			egui::EventManager<egui::MouseEvent>& getMouseBtnEventManager() override;
	};

}
