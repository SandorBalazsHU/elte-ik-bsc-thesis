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

WorkWindow::WorkWindow(void) {}

int WorkWindow::open() {
	int status = 0;
					 status = SDLInit();
	if (status == 0) status = OpenGLpreConfig();
	if (status == 0) status = openSDLWindow();
	if (status == 0) status = ImGUIinit();
	if (status == 0) status = CreateOpenGLContext();
	if (status == 0) status = OpenGLpostConfig();
	if (status == 0) status = GLEWstart();
	if (status == 0) status = ShaderConfig();
	if (status == 0) status = LoadingModels();
	if (status == 0) status = CameraConfig();
	if (status == 0) status = RenderStart();
	if (status != 0) Logger::error("WINDOW OPEN ERROR", status);
	return  status;
}

int WorkWindow::SDLInit() {
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

int WorkWindow::OpenGLpreConfig() {
	//OpenGL Configuration
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);		//Color buffer size
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);			//Red 8 bit
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);			//Green 8 bit
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);			//Blue 8 bit
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);			//Alpha 8 bit
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);		//Depth Buffer on
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);			//Depth Buffer 24
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

int WorkWindow::ImGUIinit() {
	//imgui initialisation
	bool status = ImGui_ImplSdlGL3_Init(window);
	if (status) {
		return 0;
	}else{
		Logger::error("[IMGUI START ERROR]");
		return 3;
	}
}

int WorkWindow::CreateOpenGLContext() {
	//Create OpenGL context
	context = SDL_GL_CreateContext(window);
	if (context == 0) {
		std::string SDLerror = SDL_GetError();
		Logger::error("[OpenGL context creation ERROR]: " + SDLerror);
		return 4;
	}
	return 0;
}

int WorkWindow::GLEWstart() {
	//GLEW start
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		Logger::error("[GLEW start error]");
		return 5;
	}
	return 0;
}

int WorkWindow::OpenGLpostConfig() {
	//VSINC ON
	SDL_GL_SetSwapInterval(1);

	//Check OpenGL version
	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	Logger::log("[OpenGL running]" + std::to_string(glVersion[0]) + "." + std::to_string(glVersion[1]));

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

int WorkWindow::ShaderConfig() {
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

int WorkWindow::LoadingModels() {

	//Load Texture
	//texture.FromFile("texture.png");

	// mesh betöltése
	//ball = ObjParser::parse("sphere.obj");

	return 0;
}

int WorkWindow::CameraConfig() {
	//Set Camera
	camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);
	return 0;
}

int WorkWindow::RenderStart() {
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
		while (SDL_PollEvent(&event))
		{
			//ImGUI event handling
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			bool is_mouse_captured = ImGui::GetIO().WantCaptureMouse;
			bool is_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			/*case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
				if (!is_keyboard_captured) simulation.KeyboardDown(event.key);
				break;
			case SDL_KEYUP:
				if (!is_keyboard_captured) simulation.KeyboardUp(event.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!is_mouse_captured) simulation.MouseDown(event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				if (!is_mouse_captured) simulation.MouseUp(event.button);
				break;
			case SDL_MOUSEWHEEL:
				if (!is_mouse_captured) simulation.MouseWheel(event.wheel);
				break;
			case SDL_MOUSEMOTION:
				if (!is_mouse_captured) simulation.MouseMove(event.motion);
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					simulation.Resize(event.window.data1, event.window.data2);
				}
				break;*/
			}
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
		window_title << "Rigid Body Simulation. FPS: " << fps;
		//simulation.fps[fpsPlotCounter] = fps;
		SDL_SetWindowTitle(window, window_title.str().c_str());
	}

	return 0;
}

WorkWindow::~WorkWindow(void) {
	//Cleanup
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void WorkWindow::exitWindow() {
	SDL_Quit();
	std::cout << Logger::currentDateTime() << "- Program terminated correctly" << std::endl;
}