/**
 * @name Traffic Simulation
 * @file WorkingWindow.cpp
 * @class WorkingWindow
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Opengl 3D system initialisation and window open.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "WorkWindow.h"

#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "../Control/Logger.h"
#include "Objects/ObjectStorage.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/ObjParser_OGL3.h"

/**
 * @brief OpenGL 3D Window konstructor.
*/
WorkWindow::WorkWindow(void) {
};

/**
 * @brief Open and show the window. This run all the preparation function and start the rendering.
 * Possible error codes:
 * 0 - Everithing is fine.
 * 1 - SDL Initialisation error.
 * 2 - OpenGL 3D Window creation error.
 * 3 - imGUI GUI system Initialisation error.
 * 4 - OpenGL Context creation error.
 * 5 - GLEW system initialisation error.
 * 6 - OpenGL starting or version error.
 * 7 - Shader compiling or starting error.
 * @return The starting process status number. 0 is OK Other ID-s mean errors.
*/
int WorkWindow::open() {
	int status				= sdlInit();
	if (status == 0) status = openGLpreConfig();
	if (status == 0) status = openSDLWindow();
	if (status == 0) status = imGUIinit();
	if (status == 0) status = createOpenGLContext();
	if (status == 0) status = openGLpostConfig();
	if (status == 0) status = glewStart();
	if (status == 0) status = shaderConfig();
	if (status == 0) status = renderPreconfig();
	if (status == 0) status = renderStart();
	if (status != 0) Logger::error("WINDOW OPEN ERROR", status);
	return  status;
}

/**
 * @brief SDL system initialisation.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::sdlInit() {
	//Exit Callback
	atexit(WorkWindow::exitWindow);

	//SDL Init
	int SDLstatus = SDL_Init(SDL_INIT_VIDEO);
	if (SDLstatus == -1) {
		std::string SDLerror = SDL_GetError();
		Logger::error("[SDL_Init ERROR]: " + SDLerror);
		return 1;
	}
	return 0;
}

/**
 * @brief OpenGL preconfiguration.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::openGLpreConfig() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);		//Color buffer size 32
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);			//Red 8 bit
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);			//Green 8 bit
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);			//Blue 8 bit
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);			//Alpha 8 bit
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);		//Depth Buffer on
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);			//Depth Buffer 24
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);  //Anti aliasing ON
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16); //Anti aliasing 4
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);	//Hardware accelerated
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	return 0;
}

/**
 * @brief SDL window opening.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::openSDLWindow() {
	window = 0;
	window = SDL_CreateWindow(windowDefTitle, windowDefPosX, windowDefPosY, windowDefSizeX, windowDefSizeY,
	SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	if (window == 0) {
		std::string SDLerror = SDL_GetError();
		Logger::error("[WINDOW CREATION ERROR]: " + SDLerror);
		return 2;
	}
	return 0;
}

/**
 * @brief inGUI GUS system start.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::imGUIinit() {
	bool status = GUI::init(window);
	if (status) {
		return 0;
	} else {
		Logger::error("[IMGUI START ERROR]");
		return 3;
	}
}

/**
 * @brief OpenGL Context creation.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::createOpenGLContext() {
	context = SDL_GL_CreateContext(window);
	if (context == 0) {
		std::string SDLerror = SDL_GetError();
		Logger::error("[OpenGL context creation ERROR]: " + SDLerror);
		return 4;
	}
	return 0;
}

/**
 * @brief GLEW System start.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::glewStart() {
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		Logger::error("[GLEW start error]");
		return 5;
	}
	return 0;
}

/**
 * @brief OpenGL Variables setting.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::openGLpostConfig() {
	//VSINC ON
	SDL_GL_SetSwapInterval(1);

	//Check OpenGL version
	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	Logger::log("[OpenGL running] Version: " + std::to_string(glVersion[0]) + "." + std::to_string(glVersion[1]));

	if (glVersion[0] == -1 && glVersion[1] == -1) {
		cleanup();
		Logger::error("[OpenGL starting ERROR]");
		return 6;
	}

	glClearColor(0.0f, 0.47f, 0.87f, 1.0f);	//BG Color
	glEnable(GL_CULL_FACE);					//Face test
	glEnable(GL_DEPTH_TEST);				//Depth buffer
	glEnable(GL_BLEND);						//Alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Face test
	glCullFace(GL_BACK);
	return 0;
}

/**
 * @brief OpenGL Shader configurationvand compiling.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::shaderConfig() {
	try {
		shader.createProgram();
		shader.Init({
			{ GL_VERTEX_SHADER, "View/Shaders/vertexShader.vert" },
			{ GL_FRAGMENT_SHADER, "View/Shaders/fragmentShader.frag" }
			},
		{
			{ 0, "vertexInPosition" },	// VAO 0	    --> vs_in_pos
			{ 1, "vertexInNormal" },	// VAO 1 chanel --> vs_in_normal
			{ 2, "vertexInTexture" },	// VAO 2 chanel --> vs_in_tex0
		});
	} catch (...) {
		Logger::error("[Shader config ERROR]");
		return 7;
	}
	return 0;
}

/**
 * @brief Clear the screen
*/
void WorkWindow::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Render variable setting.
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::renderPreconfig() {
	eventListener.bind(this);
	render.bind(this);
	gui.bind(this);
	objectStorage.load();
	return 0;
}

/**
 * @brief Render Start!
 * @return Status ID. 0 is OK status.
*/
int WorkWindow::renderStart() {
	while (!exit) {
		if (!objectStorage.isLoaded()) {
			if (objectStorage.loadingCheck()) objectStorage.finaliseLoading();
		}
		clearScreen();
		eventListener.eventProcessor();
		render.render();
	}

	Logger::log("Render terminated correctly!");
	cleanup();
	return 0;
}

/**
 * @brief Cleanup before closing.
*/
void WorkWindow::cleanup() {
	GUI::clean();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

/**
 * @brief OpenGL 3D Window destructor.
*/
WorkWindow::~WorkWindow(void) {
	cleanup();
}

/**
 * @brief Windows close callback function.
*/
void WorkWindow::exitWindow() {
	SDL_Quit();
}