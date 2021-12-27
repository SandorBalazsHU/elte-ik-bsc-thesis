/**
 * @name Traffic Simulation
 * @file GUI.h
 * @class GUI
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
 * Contact: sandorbalazs9402@gmail.com
*/

// Flanker-C

#pragma once
class WorkWindow;
class Render;
class EventListener;

#include <SDL.h>
#include <string>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"
#include "Objects/ObjectStorage.h"
#include "../Model/Point.h"
#include "Animator.h"

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
	 * @brief Bind the GUI system to the current working window.
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

	/**
	 * @brief Show the selected endpoint info.
	 * @param pointModelID The selected point. (Model object!)
	*/
	void showEndpointInfo(size_t pointModelID);

	/**
	 * @brief Show the selected road info.
	 * @param edgeModelID The selected edge. (Model object!)
	*/
	void showRoadInfo(size_t edgeModelID);

	/**
	 * @brief Show the selected vehicle data.
	 * @param vehicleModelID The selected vehicle. (Model object!)
	*/
	void showVehicleInfo(size_t vehicleModelID);

	/**
	 * @brief Reset the selected item info window content.
	*/
	void resetInfoWindow();

	/**
	 * @brief QuickSave function.
	*/
	void quickSave();

private:

	//Window configurators
	ImGuiWindowFlags trafficSimulationWindowFlag = 0;
	ImGuiWindowFlags mapEditorWindowFlag = 0;
	ImGuiWindowFlags pathFinderTestWindowFlag = 0;
	ImGuiWindowFlags simulationWindowFlag = 0;
	ImGuiWindowFlags graphicSettingsWindowFlag = 0;
	ImGuiWindowFlags simulationStatisticsWindowFlag = 0;

	//Window open flags.
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
	bool pathFinderTestWindowStatus = false;
	bool mapEditorWindow = true;
	bool simulationWindow = false;
	bool simulationSettingsWindowStatus = false;
	bool simulationStatisticsWindowStatus = false;
	bool helpWindowStatus = false;
	bool finaliseWarningWindowStatus = false;

	//Selection handlers
	size_t selectedStartPoint = -1;
	size_t selectedEndPoint = -1;
	size_t selectedRoad = -1;
	size_t selectedVehicle = -1;
	Uint32 pathFinderOpenTimeStamp;

	//Mode flags
	bool finalisingErrorWindow = false;
	const bool debugMode = false;

	/**
	 * @brief Marker for simulation starting.
	*/
	bool simulationStart = false;

	/**
	 * @brief Marker for paused simulations.
	*/
	bool simulationPaused = false;

	/**
	 * @brief The marker for the editor locking.
	*/
	bool editorLock = false;

	/**
	 * @brief The road path coloring counter. For pathfinder debugger window.
	*/
	size_t coloringCounter = 0;

	/**
	 * @brief The current path for pathfinder debugger window.
	*/
	std::vector<size_t> path;

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
	 * @brief The actual animator class.
	*/
	Animator* animator;

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
	 * @brief Item list.
	*/
	void itemList();

	/**
	 * @brief Car list.
	*/
	void carList(Point* point);

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
	 * @brief Pathfinder test window.
	*/
	void pathFinderTestWindow();

	/**
	 * @brief Show a help when hovered.
	 * @param desc The help text.
	*/
	void showHelpMarker(const char* desc);

	/**
	 * @brief Endpoint selector generator for start point properties.
	 * @param startPointID The current start point.
	*/
	void endpointSelector(Point* point);

	/**
	 * @brief The window for the simulation settings.
	*/
	void simulationSettingsWindow();

	/**
	 * @brief Finalize the map.
	*/
	void finalise();

	/**
	 * @brief Back to the edit mode
	*/
	void backToEditMode();

	/**
	 * @brief Start simulation
	*/
	void startSimulation();

	/**
	 * @brief Stop running simulation.
	*/
	void stopSimulation();

	/**
	 * @brief Pause running simulation.
	*/
	void pauseSimulation();

	/**
	 * @brief Window for SimulationStatistics.
	*/
	void simulationStatisticsWindow();

	/**
	 * @brief Clear the road colors.
	*/
	void clearRoadColor();

	/**
	 * @brief Close all windows.
	*/
	void closeAllWindows(std::string exeption);

	/**
	 * @brief Show help window.
	*/
	void helpWindow();

	/**
	 * @brief Show the finalizing warning window.
	*/
	void finaliseWarningWindow();
};