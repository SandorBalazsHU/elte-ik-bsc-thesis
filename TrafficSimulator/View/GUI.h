/**
 * @name Traffic Simulation
 * @file Gui.h
 * @class TrafficSimulator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The gui descriptor file, with IMgui
 * Contact: sandorbalazs9402@gmail.com
 * KSP
*/

#pragma once
class WorkWindow;
class Render;
class EventListener;

#include <SDL.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"
#include "Objects/ObjectStorage.h"

class GUI {
public:

	/**
	 * @brief The GUI constructor
	*/
	GUI(void);

	/**
	 * @brief The GUI destructor
	*/
	~GUI(void);

	/**
	 * @brief IMgui initialization and bind to the current OpenGL window.
	 * @param window The current OpenGL window for the GUI.
	 * @return The success or the error codes.
	*/
	static bool init(SDL_Window* window);

	/**
	 * @brief Stop the IMgui system.
	*/
	static void clean();

	/**
	 * @brief Bind the gui system to the current working window.
	 * @param currentWindow The current working window.
	*/
	void bind(WorkWindow* currentWindow);

	/**
	 * @brief The main GUI function. Set the GUI structure.
	*/
	void draw();

	/**
	 * @brief Render the GUI graphics in the render cycle.
	*/
	void render();

	/**
	 * @brief GUI event listener.
	 * @param event SDL Window events.
	*/
	void eventHandler(SDL_Event* event);

	/**
	 * @brief The GUI want to use the keyboard events?
	 * @return True if the GUI want to use the keyboard events.
	*/
	bool isKeyboardCaptured();

	/**
	 * @brief The GUI want to use the mouse events?
	 * @return True if the GUI want to use the mouse events.
	*/
	bool isMouseCaptured();

private:

	bool openWindow = false;

	/**
	 * @brief The current binded working window.
	*/
	WorkWindow* workingWindow;

	/**
	 * @brief The current binded OpenGL window scene.
	*/
	SDL_Window* window;

	/**
	 * @brief The current binded render.
	*/
	Render* windowRender;

	/**
	 * @brief The main preloaded object storage.
	*/
	ObjectStorage* objectStorage;

	/**
	 * @brief The current working event listener.
	*/
	EventListener* eventListener;

	/**
	 * @brief The program style tab.
	*/
	void styleTab();

	/**
	 * @brief The main menu bar description.
	*/
	void mainMenuBar();

	/**
	 * @brief The FPS graph.
	*/
	void fpsGraph();

	/**
	 * @brief Loading bar.
	*/
	void loadingBar();

	/**
	 * @brief Add new item window.
	 * @param p_open open event.
	*/
	static void addNewItemWindow(bool* p_open);

	/**
	 * @brief Item list window.
	*/
	void itemList();
};