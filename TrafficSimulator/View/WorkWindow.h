#pragma once

#include <SDL.h>
#include "Camera.h"
#include "EventListener.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"


class WorkWindow {
public:
	WorkWindow(void);
	~WorkWindow(void);
	int open();
private:
	const char* windowDefTitle = "Traffic Simulator";
	const int windowDefSizeX = 640;
	const int windowDefSizeY = 480;
	const int windowDefPosX = 100;
	const int windowDefPosY = 100;

	SDL_Window* window;

	SDL_GLContext context;

	//Shader Variables
	ProgramObject			shader;
	Camera					camera;
	EventListener			eventListener;

	Texture2D				textures;
	std::unique_ptr<Mesh>	modells;

	int sdlInit();
	int openGLpreConfig();
	int openSDLWindow();
	int imGUIinit();
	int createOpenGLContext();
	int openGLpostConfig();
	int glewStart();
	int shaderConfig();
	int loadingModels();
	int cameraConfig();
	int eventListenerConfig();
	int renderStart();

	static void exitWindow();
};