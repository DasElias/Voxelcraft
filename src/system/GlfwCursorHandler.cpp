#include "GlfwCursorHandler.h"

namespace vc {
	Lwjgl3CursorHandler::Lwjgl3CursorHandler(GLFWwindow* window) :
			window(window) {
	}

	void Lwjgl3CursorHandler::update() {
		double oldCursorX = cursorX;
		double oldCursorY = cursorY;

		glfwGetCursorPos(window, &cursorX, &cursorY);

		deltaCursorX = cursorX - oldCursorX;
		deltaCursorY = cursorY - oldCursorY;

	}

	float Lwjgl3CursorHandler::getDeltaX() {
		return float(deltaCursorX);
	}

	float Lwjgl3CursorHandler::getDeltaY() {
		return float(deltaCursorY);
	}

	float Lwjgl3CursorHandler::getCursorX() {
		return float(cursorX);
	}

	float Lwjgl3CursorHandler::getCursorY() {
		return float(cursorY);
	}

	void Lwjgl3CursorHandler::setCursorPosition(float x, float y) {
		glfwSetCursorPos(window, x, y);
	}

}
