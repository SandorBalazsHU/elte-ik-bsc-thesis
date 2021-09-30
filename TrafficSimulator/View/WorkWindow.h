#pragma once

#include <SDL.h>
#include "Camera.h"
#include "Render.h"
#include "EventListener.h"
#include "GUI.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Objects/ObjectStorage.h"

class WorkWindow {
public:
	WorkWindow(void);
	~WorkWindow(void);
	int open();

	void close() {
		exit = true;
	};

	Camera* getCamera() {
		return &camera;
	};

	ProgramObject* getShader() {
		return &shader;
	}

	SDL_Window* getWindow() {
		return window;
	}

	std::string getWindowTitle() {
		return windowTitle;
	}

	GUI* getGUI() {
		return &gui;
	}

	ObjectStorage* getObjectStorage() {
		return &objectStorage;
	}

	Render* getRender() {
		return &render;
	}

	EventListener* getEventListener() {
		return &eventListener;
	}

private:
	const char* windowDefTitle = "Traffic Simulator";
	const int windowDefSizeX = 640;
	const int windowDefSizeY = 480;
	const int windowDefPosX = 100;
	const int windowDefPosY = 100;

	std::string windowTitle = "Traffic simulator v1.0";

	SDL_Window*		window;
	SDL_GLContext	context;
	ProgramObject	shader;
	Camera			camera;
	EventListener	eventListener;
	Render			render;
	GUI				gui;
	ObjectStorage	objectStorage;

	bool exit = false;

	int sdlInit();
	int openGLpreConfig();
	int openSDLWindow();
	int imGUIinit();
	int createOpenGLContext();
	int openGLpostConfig();
	int glewStart();
	int shaderConfig();
	int renderPreconfig();
	int renderStart();

	void clearScreen();

	void cleanup();
	static void exitWindow();
};