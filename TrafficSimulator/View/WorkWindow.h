#pragma once

#include <SDL.h>
#include "Camera.h"
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
	Texture2D				texture;
	std::unique_ptr<Mesh>	ball;
	Camera					camera;

	int SDLInit();
	int OpenGLpreConfig();
	int openSDLWindow();
	int ImGUIinit();
	int CreateOpenGLContext();
	int OpenGLpostConfig();
	int GLEWstart();
	int ShaderConfig();
	int LoadingModels();
	int CameraConfig();
	int RenderStart();

	static void exitWindow();
};