/**
 * @name Traffic Simulation
 * @file WorkingWindow.h
 * @class WorkingWindow
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief OpenGL 3D system initialization and window open.
 * Contact: sandorbalazs9402@gmail.com
*/

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

/**
 * @brief The OpenGL 3D window implementation
*/
class WorkWindow {
public:

	/**
	 * @brief OpenGL 3D Window constructor.
	*/
	WorkWindow(void);

	/**
	 * @brief OpenGL 3D Window destructor.
	*/
	~WorkWindow(void);

	/**
	 * @brief Open and show the window. This run all the preparation function and start the rendering.
	 * @return The starting process status number. 0 is OK Other ID-s mean errors.
	*/
	int open();

	/**
	 * @brief Start the closing procedure.
	*/
	void close() {
		gui.close();
	};

	/**
	 * @brief Stop the render and start the closing procedure.
	*/
	void closeNow() {
		exit = true;
	};

	/**
	 * @brief Getter for the camera controller.
	 * @return pointer for the camera controller.
	*/
	Camera* getCamera() {
		return &camera;
	};

	/**
	 * @brief Getter for the 3D Window compiled shader program.
	 * @return The OpõenGL 3D Shader.
	*/
	ProgramObject* getShader() {
		return &shader;
	}

	/**
	 * @brief Getter for the SDL Windows object.
	 * @return The SDL OpenGL window object.
	*/
	SDL_Window* getWindow() {
		return window;
	}

	/**
	 * @brief Getter for the window title.
	 * @return The window title.
	*/
	std::string getWindowTitle() {
		return windowTitle;
	}

	/**
	 * @brief Getter for the GUI representer.
	 * @return Pointer to the GUI controller class.
	*/
	GUI* getGUI() {
		return &gui;
	}

	/**
	 * @brief Getter for the object storage.
	 * @return Pointer to the object storage.
	*/
	ObjectStorage* getObjectStorage() {
		return &objectStorage;
	}

	/**
	 * @brief Getter for the render object.
	 * @return Pointer to the current render.
	*/
	Render* getRender() {
		return &render;
	}

	/**
	 * @brief Getter for the event listener.
	 * @return Pointer for the current event listener.
	*/
	EventListener* getEventListener() {
		return &eventListener;
	}

private:

	/**
	 * @brief The main window default tittle text.
	*/
	const char* windowDefTitle = "Traffic Simulator";

	/**
	 * @brief The default windows with.
	*/
	const int windowDefSizeX = 640;

	/**
	 * @brief The default window height.
	*/
	const int windowDefSizeY = 480;

	/**
	 * @brief The default window horizontal position.
	*/
	const int windowDefPosX = 100;

	/**
	 * @brief The default window vertical position.
	*/
	const int windowDefPosY = 100;

	/**
	 * @brief The main window base tittle text.
	*/
	std::string windowTitle = "Traffic simulator";

	/**
	 * @brief The SDL window pointer.
	*/
	SDL_Window*		window;

	/**
	 * @brief The openGL context object.
	*/
	SDL_GLContext	context;

	/**
	 * @brief The shader object.
	*/
	ProgramObject	shader;

	/**
	 * @brief The camera controller object.
	*/
	Camera			camera;

	/**
	 * @brief The event listener object.
	*/
	EventListener	eventListener;

	/**
	 * @brief The render object.
	*/
	Render			render;

	/**
	 * @brief The GUI controller object.
	*/
	GUI				gui;

	/**
	 * @brief The 3D Object storage object.
	*/
	ObjectStorage	objectStorage;

	/**
	 * @brief The render loop running parameter.
	*/
	bool exit = false;

	/**
	 * @brief SDL system initialization.
	 * @return Status ID. 0 is OK status. 
	*/
	int sdlInit();

	/**
	 * @brief OpenGL pre-configuration.
	 * @return Status ID. 0 is OK status.
	*/
	int openGLpreConfig();

	/**
	 * @brief SDL window opening.
	 * @return Status ID. 0 is OK status.
	*/
	int openSDLWindow();

	/**
	 * @brief inGUI GUS system start.
	 * @return Status ID. 0 is OK status.
	*/
	int imGUIinit();

	/**
	 * @brief OpenGL Context creation.
	 * @return Status ID. 0 is OK status.
	*/
	int createOpenGLContext();

	/**
	 * @brief OpenGL Variables setting.
	 * @return Status ID. 0 is OK status.
	*/
	int openGLpostConfig();

	/**
	 * @brief GLEW System start.
	 * @return Status ID. 0 is OK status.
	*/
	int glewStart();

	/**
	 * @brief OpenGL Shader configuration compiling.
	 * @return Status ID. 0 is OK status.
	*/
	int shaderConfig();

	/**
	 * @brief Render variable setting.
	 * @return Status ID. 0 is OK status.
	*/
	int renderPreconfig();

	/**
	 * @brief Render Start!
	 * @return Status ID. 0 is OK status.
	*/
	int renderStart();

	/**
	 * @brief Clear the screen
	*/
	void clearScreen();

	/**
	 * @brief Cleanup before closing.
	*/
	void cleanup();

	/**
	 * @brief Windows close callback function.
	*/
	static void exitWindow();
};