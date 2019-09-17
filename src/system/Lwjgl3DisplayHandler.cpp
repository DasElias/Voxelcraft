#include "Lwjgl3DisplayHandler.h"

namespace vc {
	Lwjgl3DisplayHandler::Lwjgl3DisplayHandler(GLFWwindow* window, int startWidth, int startHeight) :
			window(window),
			width(startWidth),
			height(startHeight),
			framebufferWidth(startWidth),
			framebufferHeight(startHeight),
			contentScaleX(1),
			contentScaleY(1) {
	}

	void Lwjgl3DisplayHandler::update() {
		glfwGetWindowSize(window, &width, &height);
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glfwGetWindowContentScale(window, &contentScaleX, &contentScaleY);
	}

	int Lwjgl3DisplayHandler::getWidth() {
		return width;
	}

	int Lwjgl3DisplayHandler::getHeight() {
		return height;
	}

	int Lwjgl3DisplayHandler::getFramebufferWidth() {
		return framebufferWidth;
	}

	int Lwjgl3DisplayHandler::getFramebufferHeight() {
		return framebufferHeight;
	}

	float Lwjgl3DisplayHandler::getContentScaleX() {
		return contentScaleX;
	}

	float Lwjgl3DisplayHandler::getContentScaleY() {
		return contentScaleY;
	}

}

