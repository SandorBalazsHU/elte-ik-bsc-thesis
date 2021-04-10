#include "WorkWindow.h";

#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "../Control/Logger.h";
#include "Utilities/ProgramObject.h"
#include "Utilities/ObjParser_OGL3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

WorkWindow::WorkWindow(void) {
}

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
	if (status == 0) status = cameraConfig();
	if (status == 0) status = eventListenerConfig();
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
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
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

int WorkWindow::cameraConfig() {
	//Set Camera
	camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);
	return 0;
}

int WorkWindow::eventListenerConfig() {
	eventListener.init(camera);
	return 0;
}

int WorkWindow::renderStart() {
	//Base variables for the rander loop
	bool quit = false;
	SDL_Event event;
	int fpsPlotCounter = 0;

	//The render loop
	while (!quit)
	{
		//The FPS Counter
		fpsPlotCounter++;
		if (fpsPlotCounter == 100) fpsPlotCounter = 0;
		const Uint32 time = SDL_GetTicks();

		//Event processing loop
		while (SDL_PollEvent(&event)) {
			//ImGUI event handling
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			bool isMouseEventCaptured = ImGui::GetIO().WantCaptureMouse;
			bool isKeyEventCaptured = ImGui::GetIO().WantCaptureKeyboard;
			//Program eventlistener
			if (!isMouseEventCaptured && !isKeyEventCaptured) quit = eventListener.event(event);
		}

		//Renderinga
		//ImGui_ImplSdlGL3_NewFrame(window);
		//simulation.Update();
		camera.Update();
		//simulation.Render();
		glEnable(GL_CULL_FACE); //Face test ON
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Use();
		//ImGui::Render();
		SDL_GL_SwapWindow(window);

		//FPS Counter
		const Uint32 last_time = SDL_GetTicks();
		std::stringstream window_title;
		float fps = 1000.0f / (last_time - time);
		window_title << "Traffic Simulation. FPS: " << fps;
		//simulation.fps[fpsPlotCounter] = fps;
		SDL_SetWindowTitle(window, window_title.str().c_str());
	}

	Logger::log("Render terminated correctly!");
	return 0;
}

WorkWindow::~WorkWindow(void) {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void WorkWindow::exitWindow() {
	SDL_Quit();
}