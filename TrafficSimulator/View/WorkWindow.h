#pragma once

#include <SDL.h>
#include "Camera.h"
#include "EventListener.h"
#include "Render.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"



class WorkWindow {
public:
	WorkWindow(void);
	~WorkWindow(void);
	int open();

	int close() {
		exit = true;
	}

	Camera* getCamera() {
		return &camera;
	}
private:
	const char* windowDefTitle = "Traffic Simulator";
	const int windowDefSizeX = 640;
	const int windowDefSizeY = 480;
	const int windowDefPosX = 100;
	const int windowDefPosY = 100;

	SDL_Window*		window;
	SDL_GLContext	context;
	ProgramObject	shader;
	Camera			camera;
	//EventListener	eventListener;
	//Render			render;

	bool exit = false;

	int sdlInit();
	int openGLpreConfig();
	int openSDLWindow();
	int imGUIinit();
	int createOpenGLContext();
	int openGLpostConfig();
	int glewStart();
	int shaderConfig();
	int loadingModels();
	int renderStart();

	void cleanup();
	static void exitWindow();
};