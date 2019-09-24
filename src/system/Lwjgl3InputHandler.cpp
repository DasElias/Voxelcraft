#include "Lwjgl3InputHandler.h"
#include "model/events/ScrollEvent.h"

#include <map>

namespace vc {
	namespace {
		static std::map<GLFWwindow*, Lwjgl3InputHandler*> instanceMap;

		void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
			egui::MouseEvent evt = { button, action == GLFW_PRESS , int(egui::getCursorHandler().getCursorX()), int(egui::getCursorHandler().getCursorY())};
			instanceMap.at(window)->getMouseBtnEventManager().fireEvent(evt);
		}

		void  scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
			if (yoffset != 0) {
				egui::ScrollEvent evt = { float(yoffset) };
				instanceMap.at(window)->getScrollEventManager().fireEvent(evt);
			}
		}
	}


	Lwjgl3InputHandler::Lwjgl3InputHandler(GLFWwindow* window) :
			window(window) {
		
		instanceMap.insert(std::make_pair(window, this));
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetMouseButtonCallback(window, mouseBtnCallback);
	}

	Lwjgl3InputHandler::~Lwjgl3InputHandler() {
		glfwSetScrollCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		
		instanceMap.erase(window);
	}



	bool Lwjgl3InputHandler::isKeyDown(int key) {
		return ((key < 10) ? glfwGetMouseButton(window, key) : glfwGetKey(window, key)) == GLFW_PRESS;
	}

	void Lwjgl3InputHandler::setCursorInputMode(int cursorMode) {
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
	}

	egui::EventManager<egui::ScrollEvent>& Lwjgl3InputHandler::getScrollEventManager() {
		return scrollEventManager;
	}

	egui::EventManager<egui::MouseEvent>& Lwjgl3InputHandler::getMouseBtnEventManager() {
		return mouseBtnEventManager;
	}

}
