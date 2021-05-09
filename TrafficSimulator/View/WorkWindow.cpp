#include "WorkWindow.h";
#include "Render.h"
#include "EventListener.h"

#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "../Control/Logger.h";
#include "Utilities/ProgramObject.h"
#include "Utilities/ObjParser_OGL3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

WorkWindow::WorkWindow(void) {
};

int WorkWindow::open() {
	int status				= sdlInit();
	if (status == 0) status = openGLpreConfig();
	if (status == 0) status = openSDLWindow();
	if (status == 0) status = imGUIinit();
	if (status == 0) status = createOpenGLContext();
	if (status == 0) status = openGLpostConfig();
	if (status == 0) status = glewStart();
	if (status == 0) status = shaderConfig();
	if (status == 0) status = loadingModels();
	if (status == 0) status = renderStart();
	if (status != 0) Logger::error("WINDOW OPEN ERROR", status);
	return  status;
}

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

int WorkWindow::openGLpreConfig() {
	//OpenGL Configuration
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);		//Color buffer size 32
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);			//Red 8 bit
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);			//Green 8 bit
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);			//Blue 8 bit
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);			//Alpha 8 bit
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);		//Depth Buffer on
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);			//Depth Buffer 24
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);  //Anti aliasing ON
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);  //Anti aliasing 4
	return 0;
}

int WorkWindow::openSDLWindow() {
	//Creating Window
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

int WorkWindow::imGUIinit() {
	//imgui initialisation
	bool status = ImGui_ImplSdlGL3_Init(window);
	if (status) {
		return 0;
	}else{
		Logger::error("[IMGUI START ERROR]");
		return 3;
	}
}

int WorkWindow::createOpenGLContext() {
	//Create OpenGL context
	context = SDL_GL_CreateContext(window);
	if (context == 0) {
		std::string SDLerror = SDL_GetError();
		Logger::error("[OpenGL context creation ERROR]: " + SDLerror);
		return 4;
	}
	return 0;
}

int WorkWindow::glewStart() {
	//GLEW start
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		Logger::error("[GLEW start error]");
		return 5;
	}
	return 0;
}

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

	return 0;
}

int WorkWindow::shaderConfig() {
	//Shader loading
	try {
		//ProgramObject	shader;
		shader.createProgram();
		shader.Init({
			{ GL_VERTEX_SHADER, "View/Shaders/vertexShader.vert" },
			{ GL_FRAGMENT_SHADER, "View/Shaders/fragmentShader.frag" }
			},
		{
			{ 0, "vs_in_pos" },		// VAO 0	    --> vs_in_pos
			{ 1, "vs_in_normal" },	// VAO 1 chanel --> vs_in_normal
			{ 2, "vs_out_tex0" },	// VAO 2 chanel --> vs_in_tex0
		});
	} catch (...) {
		Logger::error("[Shader config ERROR]");
		return 7;
	}
	return 0;
}

int WorkWindow::loadingModels() {

	//Load Texture
	//texture.FromFile("texture.png");

	// mesh betöltése
	//ball = ObjParser::parse("sphere.obj");

	return 0;
}

void WorkWindow::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int WorkWindow::renderStart() {
	EventListener	eventListener;
	Render			render;
	eventListener.bind(this);
	render.bind(this);

	//The render loop
	while (!exit) {
		clearScreen();
		eventListener.eventProcessor();
		render.render();
	}

	Logger::log("Render terminated correctly!");
	cleanup();
	return 0;
}

void WorkWindow::cleanup() {
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

WorkWindow::~WorkWindow(void) {
	cleanup();
}

void WorkWindow::exitWindow() {
	SDL_Quit();
}