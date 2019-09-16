#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Main.h"

#include <input/IOHandler.h>

#include <iostream>
#include <stdexcept>

#include "../model/states/IngameState.h"
#include "../model/states/MainMenuState.h"
#include "../model/states/PauseState.h"
#include "../model/states/StateManager.h"

#include "../model/utils/FileUtils.h"
#include "../model/utils/TimeUtils.h"
#include "../rendering/LevelRenderer.h"
#include "../renderModel/ChunkVaoManager.h"
#include "KeyAssignmentsImpl.h"
#include "Lwjgl3CursorHandler.h"
#include "Lwjgl3DisplayHandler.h"
#include "Lwjgl3InputHandler.h"

#include <boost/filesystem.hpp>
#include <static_init.h>
#include <system\EGuiContext.h>
#include <rendering/NvgRenderer.h>
#include <model/utils/Font.h>

#include "..//eguiImpl/themes/VoxelcraftTheme.h"
#include <themes/ThemeManager.h>
#include <model\nodes\Button.cpp>

using namespace std;
using namespace vc::model;
using namespace vc::rendering;
using namespace vc::renderingModel;


void glfwErrorCallback(int error, const char* description) {
	cerr << "--------------- GLFW-ERROR ---------------" << endl;
	cerr << endl;

	cerr << error << endl;
	cerr << description << endl;

	cerr << endl;
	cerr << "------------------------------------------";
	exit(-1);
}

void cppExceptionCallback() {
	cerr << "--------------- Exception ----------------";
	cerr << endl;

	if(auto exc = std::current_exception()) {
		// we have an exception
		try {
			std::rethrow_exception(exc);
		} catch(const std::exception& exception) {
			cerr << exception.what();
		} catch(...) {
			cerr << "Unknown Exception!";
		}
	}

	cerr << endl;
	cerr << "------------------------------------------";
	exit(-1);
}


static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if(type == GL_DEBUG_TYPE_OTHER) return;

	cerr << "------------- OpenGL-Callback ------------" << endl;
	cerr << "message: "<< message << endl;
	cerr << "type: ";
	    switch (type) {
	    case GL_DEBUG_TYPE_ERROR:
	        cerr << "ERROR";
	        break;
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	        cerr << "DEPRECATED_BEHAVIOR";
	        break;
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	        cerr << "UNDEFINED_BEHAVIOR";
	        break;
	    case GL_DEBUG_TYPE_PORTABILITY:
	        cerr << "PORTABILITY";
	        break;
	    case GL_DEBUG_TYPE_PERFORMANCE:
	        cerr << "PERFORMANCE";
	        break;
	    case GL_DEBUG_TYPE_OTHER:
	        cerr << "OTHER";
	        break;
	}
	cerr << endl;

	cerr << "id: " << id << endl;
	cerr << "severity: ";
	switch (severity) {
	    case GL_DEBUG_SEVERITY_LOW:
	        cerr << "LOW";
	        break;
	    case GL_DEBUG_SEVERITY_MEDIUM:
	        cerr << "MEDIUM";
	        break;
	    case GL_DEBUG_SEVERITY_HIGH:
	        cerr << "HIGH";
	        break;
	    }
	cerr << endl;
	cerr << "------------------------------------------";

}

namespace vc::system {
	Main::Main() {


		// set exception handlers
		std::set_terminate(cppExceptionCallback);
		glfwSetErrorCallback(glfwErrorCallback);

		// TODO log program start

		if(! glfwInit()) throw std::runtime_error("unable to initialize GLFW");

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_SAMPLES, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		p_window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "VoxelCraft", NULL, NULL);

		Lwjgl3DisplayHandler displayHandler(p_window, START_WIDTH, START_HEIGHT);
		Lwjgl3InputHandler inputHandler(p_window);
		Lwjgl3CursorHandler cursorHandler(p_window);
		KeyAssignmentsImpl keyAssignments;

		cursorHandler.setCursorPosition(START_WIDTH / 2.0f, START_HEIGHT / 2.0f);
		egui::init(displayHandler, inputHandler, cursorHandler, keyAssignments);

		glfwMakeContextCurrent(p_window);
		gladLoadGL();		


		glfwShowWindow(p_window);
		glfwSetWindowPos(p_window, 20, 20);
		glfwFocusWindow(p_window);

		glfwSwapInterval(1);

		glDebugMessageCallback( MessageCallback, 0 );
		glEnable( GL_DEBUG_OUTPUT );
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_CLAMP);
		glClearColor(0, 0, 0, 1);

		int programId=glCreateProgram();


		egui::EGuiContext ctx = {};
		egui::NvgRenderer nvgRenderer(ctx);
		egui::Font::setDefaultFont(egui::Font::createSystemFont(ctx, "Arial"));

		egui::VoxelcraftTheme theme(ctx);
		egui::ThemeManager::getInstance().addTheme(&theme);

		static_init::execute();

		LevelRenderer levelRenderer;
		ChunkVaoManager vaoManager(400);
		ctpl::thread_pool threadPool(2);

		MainMenuState mainMenuState(stateManager, vaoManager, threadPool, nvgRenderer, ctx);
		IngameState ingameState(stateManager, levelRenderer, *this);
		PauseState pauseState(stateManager, levelRenderer, nvgRenderer, ctx);

		stateManager.addState("MainMenuState", &mainMenuState);
		stateManager.addState("IngameState", &ingameState);
		stateManager.addState("PauseState", &pauseState);
		stateManager.changeState("MainMenuState");

		loop();

	}

	Main::~Main() {
		glfwDestroyWindow(p_window);
		glfwTerminate();

	}

	float Main::getFps() const {
		return fps;
	}

	void Main::loop() {
		long long lastFrameTime = getMilliseconds();

		while(! glfwWindowShouldClose(p_window)) {
			long long thisFrameTime = getMilliseconds();

			float deltaSeconds = (thisFrameTime - lastFrameTime) / 1000.0f;
			lastFrameTime = thisFrameTime;

			this->fps = 1.0f / deltaSeconds;


			// clear color and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// update IO handler
			egui::update(deltaSeconds);


			stateManager.updateAndRender(deltaSeconds);


			glfwSwapInterval(1);
			glfwSwapBuffers(p_window);
			glfwPollEvents();
		}
	}



}


int main() {
	std::cerr << "Application has started!" << endl;
	vc::system::Main();
	return 1;
}
