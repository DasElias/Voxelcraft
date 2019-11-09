#pragma once

#include <GLFW/glfw3.h>
#include "input/IOHandler.h"

namespace vc {

	class Lwjgl3InputHandler : public egui::InputHandler {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			GLFWwindow* window;
			egui::EventManager<egui::ScrollEvent> scrollEventManager;
			egui::EventManager<egui::MouseEvent> mouseBtnEventManager;
			egui::EventManager<egui::KeyEvent> keyEventManager;
			egui::EventManager<egui::CharEnterEvent> charEnteredEventManager;

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

			std::string getKeyName(int key, int scanCode) override;
			std::string getKeyChar(int key, int scanCode) override;

			egui::EventManager<egui::ScrollEvent>& getScrollEventManager() override;
			egui::EventManager<egui::MouseEvent>& getMouseBtnEventManager() override;
			egui::EventManager<egui::KeyEvent>& getKeyEventManager() override;
			egui::EventManager<egui::CharEnterEvent>& getCharEnteredEventManager() override;
	};

}
