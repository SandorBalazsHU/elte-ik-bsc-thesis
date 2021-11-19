/**
 * @name Traffic Simulation
 * @file GUI.h
 * @class GUI
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
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

	/**
	 * @brief Application closing checker window.
	*/
	void close();

private:
	ImGuiWindowFlags trafficSimulationWindowFlag = 0;
	ImGuiWindowFlags mapEditorWindowFlag = 0;
	bool openWindowStatus = false;
	bool saveWindowStatus = false;
	bool saveAsWindowStatus = false;
	bool ImGuiSettingsWindowStatus = false;
	bool newMapConfirmWindowStatus = false;
	bool controlsWindowStatus = false;
	bool aboutWindowStatus = false;
	bool closingCheckerWindowStatus = false;
	bool graphicSettingsWindowStatus = false;
	bool debugOptionsWindowStatus = false;
	bool runningStatisticsWindowStatus = false;

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
	 * @brief The main menu bar description.
	*/
	void mainMenuBar();

	/**
	 * @brief The widows opening and closing system.
	*/
	void windowHandler();

	/**
	 * @brief The FPS graph.
	*/
	void fpsGraph();

	/**
	 * @brief Description of the opening window.
	*/
	void openWindow();
	
	/**
	 * @brief the save window descriptor.
	*/
	void saveWindow();

	/**
	 * @brief the save as window descriptor.
	*/
	void saveAsWindow();

	/**
	 * @brief Window for the new map confirmation.
	*/
	void newMapConfirmWindow();

	/**
	 * @brief Item list window.
	*/
	void itemList();

	/**
	 * @brief Window for the controls tutorial.
	*/
	void controlsWindow();

	/**
	 * @brief Window for the about tab.
	*/
	void aboutWindow();

	/**
	 * @brief Application closing checker window.
	*/
	void closeWindow();

	/**
	 * @brief Window descriptor for the graphic settings window.
	*/
	void graphicSettingsWindow();

	/**
	 * @brief Window for the debug options.
	*/
	void debugOptionsWindow();

	/**
	 * @brief Window for the running statistics.
	*/
	void runningStatisticsWindow();

	/**
	 * @brief Show a help when howered.
	 * @param desc The help text.
	*/
	void showHelpMarker(const char* desc);
};