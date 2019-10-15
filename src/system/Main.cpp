#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Main.h"

#include <input/IOHandler.h>

#include <stdexcept>

#include "../model/states/IngameState.h"
#include "../model/states/KeyBindingsState.h"
#include "../model/states/MainMenuState.h"
#include "../model/states/PauseState.h"
#include "../model/states/OptionsState.h"
#include "../model/states/SelectLevelState.h"
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
#include <model/utils/Image.h>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

using namespace std;
using namespace vc;
using namespace vc;
using namespace vc;


void glfwErrorCallback(int error, const char* description) {
	BOOST_LOG_TRIVIAL(fatal) << "--------------- GLFW-ERROR ---------------";
	BOOST_LOG_TRIVIAL(fatal) << error;
	BOOST_LOG_TRIVIAL(fatal) << description;
	BOOST_LOG_TRIVIAL(fatal) << "------------------------------------------";
	exit(-1);
}

void cppExceptionCallback() {
	BOOST_LOG_TRIVIAL(fatal) << "--------------- Exception ----------------";

	if(auto exc = std::current_exception()) {
		// we have an exception
		try {
			std::rethrow_exception(exc);
		} catch(const std::exception& exception) {
			BOOST_LOG_TRIVIAL(fatal) << exception.what();
		} catch(...) {
			BOOST_LOG_TRIVIAL(fatal) << "Unknown Exception!";
		}
	}

	BOOST_LOG_TRIVIAL(fatal) << "------------------------------------------";
	exit(-1);
}


static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if(type == GL_DEBUG_TYPE_OTHER) return;

	BOOST_LOG_TRIVIAL(fatal) << "------------- OpenGL-Callback ------------";
	BOOST_LOG_TRIVIAL(fatal) << "message: ";
	BOOST_LOG_TRIVIAL(fatal) << "\t" << message;
	BOOST_LOG_TRIVIAL(fatal) << "type: ";
	    switch (type) {
	    case GL_DEBUG_TYPE_ERROR:
			BOOST_LOG_TRIVIAL(fatal) << "\tERROR";
	        break;
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			BOOST_LOG_TRIVIAL(fatal) << "\tDEPRECATED_BEHAVIOR";
	        break;
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			BOOST_LOG_TRIVIAL(fatal) << "\tUNDEFINED_BEHAVIOR";
	        break;
	    case GL_DEBUG_TYPE_PORTABILITY:
			BOOST_LOG_TRIVIAL(fatal) << "\tPORTABILITY";
	        break;
	    case GL_DEBUG_TYPE_PERFORMANCE:
			BOOST_LOG_TRIVIAL(fatal) << "\tPERFORMANCE";
	        break;
	}
	BOOST_LOG_TRIVIAL(fatal) << "id:";
	BOOST_LOG_TRIVIAL(fatal) << "\t" << id;
	BOOST_LOG_TRIVIAL(fatal) << "severity:";
	switch (severity) {
	    case GL_DEBUG_SEVERITY_LOW:
			BOOST_LOG_TRIVIAL(fatal) << "\tLOW";
	        break;
	    case GL_DEBUG_SEVERITY_MEDIUM:
			BOOST_LOG_TRIVIAL(fatal) << "\tMEDIUM";
	        break;
	    case GL_DEBUG_SEVERITY_HIGH:
			BOOST_LOG_TRIVIAL(fatal) << "\tHIGH";
	        break;
	    }
	BOOST_LOG_TRIVIAL(fatal) << "------------------------------------------";

}

void initLogging() {
	static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");
	boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

	// Output message to console
	boost::log::add_console_log(
		std::cerr,
		boost::log::keywords::format = COMMON_FMT,
		boost::log::keywords::auto_flush = true
	);

	// Output message to file
	boost::log::add_file_log(
		boost::log::keywords::target = vc::getApplicationFolder() + "\\logs",
		boost::log::keywords::file_name = "vclog_%3N.log",
		boost::log::keywords::rotation_size = 1 * 1024 * 1024,
		boost::log::keywords::max_size = 20 * 1024 * 1024,
		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
		boost::log::keywords::format = COMMON_FMT,
		boost::log::keywords::auto_flush = true
	);

	boost::log::add_common_attributes();

	boost::log::core::get()->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::info
	);
}

namespace vc {
	Main::Main() {
		// initialize boost trivial logging
		initLogging();

		// log application start
		BOOST_LOG_TRIVIAL(info) << "Application has started!";

		// set exception handlers
		std::set_terminate(cppExceptionCallback);
		glfwSetErrorCallback(glfwErrorCallback);

		if(! glfwInit()) throw std::runtime_error("unable to initialize GLFW");

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		//glfwWindowHint(GLFW_SAMPLES, 2);
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

		glfwSwapInterval(VSYNC);

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
		egui::Image::setContext(ctx);
		egui::Font::setDefaultFont(egui::Font::createSystemFont(ctx, "Arial"));

		egui::VoxelcraftTheme theme(ctx);
		egui::ThemeManager::getInstance().addTheme(&theme);

		static_init::execute();

		LevelRenderer levelRenderer(nvgRenderer, ctx);
		ChunkVaoManager vaoManager(400);
		ctpl::thread_pool threadPool(2);

		MainMenuState mainMenuState(stateManager, vaoManager, threadPool, nvgRenderer, ctx);
		IngameState ingameState(stateManager, levelRenderer, nvgRenderer);
		PauseState pauseState(stateManager, levelRenderer, nvgRenderer, ctx);
		OptionsState optionsState(stateManager, nvgRenderer, ctx);
		KeyBindingsState keyBindingsState(stateManager, nvgRenderer, ctx);
		SelectLevelState selectLevelState(stateManager, nvgRenderer, vaoManager, threadPool);

		stateManager.addState("MainMenuState", &mainMenuState);
		stateManager.addState("IngameState", &ingameState);
		stateManager.addState("PauseState", &pauseState);
		stateManager.addState("OptionsState", &optionsState);
		stateManager.addState("KeyBindingsState", &keyBindingsState);
		stateManager.addState("SelectLevelState", &selectLevelState);
		stateManager.changeState("MainMenuState");

		loop();

	}

	Main::~Main() {
		glfwDestroyWindow(p_window);
		glfwTerminate();

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


			glfwSwapInterval(VSYNC);
			glfwSwapBuffers(p_window);
			glfwPollEvents();
		}
	}



}


int main() {
	vc::Main();
	return 1;
}
