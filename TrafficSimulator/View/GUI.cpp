/**
 * @name Traffic Simulation
 * @file GUI.cpp
 * @class GUI
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
 * Contact: sandorbalazs9402@gmail.com
*/

// sce to aux
// alarm 1202

#include "GUI.h"
#include "WorkWindow.h"
#include "Render.h"
#include "EventListener.h"
#include "fpsCounter.h"
#include "../Model/Point.h"
#include "../Model/Vehicle.h"
#include <sstream>

/**
 * @brief The GUI constructor
*/
GUI::GUI(void) {

}

/**
 * @brief The GUI destructor
*/
GUI::~GUI(void) {

}

/**
 * @brief IMgui initialization and bind to the current OpenGL window.
 * @param window The current OpenGL window for the GUI.
 * @return The success or the error codes.
*/
bool GUI::init(SDL_Window* window) {
	ImGuiStyle& style = ImGui::GetStyle();
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.FrameRounding = 3;
	style.FrameBorderSize = 1;
	return ImGui_ImplSdlGL3_Init(window);
}

/**
 * @brief Stop the IMgui system.
*/
void GUI::clean() {
	ImGui_ImplSdlGL3_Shutdown();
}

/**
 * @brief Bind the GUI system to the current working window.
 * @param currentWindow The current working window.
*/
void GUI::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	window = currentWindow->getWindow();
	objectStorage = workingWindow->getObjectStorage();
	windowRender = currentWindow->getRender();
	eventListener = currentWindow->getEventListener();
	animator = windowRender->getAnimator();
}

/**
 * @brief GUI event listener.
 * @param event SDL Window events.
*/
void GUI::eventHandler(SDL_Event* event) {
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

/**
 * @brief The GUI want to use the keyboard events?
 * @return True if the GUI want to use the keyboard events.
*/
bool GUI::isKeyboardCaptured() {
	return ImGui::GetIO().WantCaptureMouse;
}

/**
 * @brief The GUI want to use the mouse events?
 * @return True if the GUI want to use the mouse events.
*/
bool GUI::isMouseCaptured() {
	return ImGui::GetIO().WantCaptureKeyboard;
}

/**
 * @brief The main menu bar description.
*/
void GUI::mainMenuBar() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl+N", &newMapConfirmWindowStatus, !editorLock)) {}
			if (ImGui::MenuItem("Open", "Ctrl+O", &openWindowStatus, !editorLock)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S", &saveWindowStatus)) {}
			if (ImGui::MenuItem("Save As..", "Ctrl+A", &saveAsWindowStatus)) {}
			if (ImGui::MenuItem("Close", "Ctrl+X", &closingCheckerWindowStatus)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Graphics settings", "CTRL+G", &graphicSettingsWindowStatus)) { closeAllWindows("graphicSettingsWindowStatus"); }
			if (ImGui::MenuItem("ImGui settings", "CTRL+I", &ImGuiSettingsWindowStatus)) { closeAllWindows("ImGuiSettingsWindowStatus"); }
			ImGui::Separator();
			if (ImGui::MenuItem("Running statistics", "CTRL+R", &runningStatisticsWindowStatus)) { closeAllWindows("runningStatisticsWindowStatus"); }
			if (ImGui::MenuItem("Debug options", "CTRL+D", &debugOptionsWindowStatus)) { closeAllWindows("debugOptionsWindowStatus"); }
			if (ImGui::MenuItem("Pathfinder algorithm test.", "CTRL+P", &pathFinderTestWindowStatus, editorLock)) { closeAllWindows("pathFinderTestWindowStatus");  pathFinderOpenTimeStamp = SDL_GetTicks(); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Simulation")) {
			if (ImGui::MenuItem("Finalizing", "CTRL+Q", false, !editorLock)) { finalise(); }
			if (ImGui::MenuItem("Edit mode", "CTRL+Q", false, (editorLock && !simulationStart))) { closeAllWindows("");  backToEditMode(); }
			if (ImGui::MenuItem("START Simulation", "CTRL+Q", false, (!simulationStart && editorLock))) { startSimulation(); }
			if (!simulationPaused) {
				if (ImGui::MenuItem("PAUSE Simulation", "CTRL+W", false, (simulationStart && editorLock))) { pauseSimulation(); }
			} else {
				if (ImGui::MenuItem("Continue Simulation", "CTRL+W", false, (simulationStart && editorLock))) { startSimulation(); }
			}
			if (ImGui::MenuItem("STOP Simulation", "CTRL+W", false, (simulationStart && editorLock))) { stopSimulation(); }
			ImGui::Separator();
			if (ImGui::MenuItem("Simulation settings", "CTRL+E", &simulationSettingsWindowStatus, editorLock)) { closeAllWindows("simulationSettingsWindowStatus"); }
			ImGui::Separator();
			if (ImGui::MenuItem("Simulation statistics", "CTRL+f", &simulationStatisticsWindowStatus, editorLock)) { closeAllWindows("simulationStatisticsWindowStatus"); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Help", "CTRL+H", &helpWindowStatus)) {}
			if (ImGui::MenuItem("Controls", "CTRL+C", &controlsWindowStatus)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("About", "CTRL+K", &aboutWindowStatus)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

/**
 * @brief The widows opening and closing system.
*/
void GUI::windowHandler() {
	if (openWindowStatus) openWindow();
	if (saveWindowStatus) saveWindow();
	if (saveAsWindowStatus) saveAsWindow();
	if (newMapConfirmWindowStatus) newMapConfirmWindow();
	if (controlsWindowStatus) controlsWindow();
	if (aboutWindowStatus) aboutWindow();
	if (closingCheckerWindowStatus) closeWindow();
	if (graphicSettingsWindowStatus) graphicSettingsWindow();
	if (debugOptionsWindowStatus) debugOptionsWindow();
	if (runningStatisticsWindowStatus) runningStatisticsWindow();
	if (pathFinderTestWindowStatus) pathFinderTestWindow();
	if (simulationSettingsWindowStatus) simulationSettingsWindow();
	if (simulationStatisticsWindowStatus) simulationStatisticsWindow();
	if (finaliseWarningWindowStatus) finaliseWarningWindow();
	if (helpWindowStatus) helpWindow();
	if (ImGuiSettingsWindowStatus) { ImGui::Begin("ImGui Style Editor", &ImGuiSettingsWindowStatus); ImGui::ShowStyleEditor(); ImGui::End(); }
}

/**
 * @brief The FPS graph.
*/
void GUI::fpsGraph() {
	static int graphTop = 150;
	std::ostringstream ss;
	ss << "Average FPS: " << fpsCounter::getAverageFPS();
	ImGui::Text(ss.str().c_str());
	ImGui::Text("ImGUI average: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::PushItemWidth(-1);
	ImGui::PlotLines("FPS", fpsCounter::fpsLog, IM_ARRAYSIZE(fpsCounter::fpsLog), 0, "FPS", 0.0f, graphTop, ImVec2(0, 80));
	ImGui::PlotHistogram("FP2S", fpsCounter::fpsLog, IM_ARRAYSIZE(fpsCounter::fpsLog), 0, "FPS2", 0.0f, graphTop, ImVec2(0, 80));
	ImGui::InputInt("Graph top limit", &graphTop);
}

/**
 * @brief Item list.
*/
void GUI::itemList() {
	int i = 0;
	std::map<int, Object3D>::iterator it = objectStorage->object3Ds.begin();
	while (it != objectStorage->object3Ds.end()) {
		ImVec2 size = ImVec2(100, 100);
			if (i < 2) {
				size = ImVec2(42, 42);
				ImGui::SameLine();
			} else {
				size = ImVec2(100, 100);
			}
		if (it->second.getType() == "object") {
			if (ImGui::ImageButton((void*)(intptr_t)it->second.getIcon(), size)) {
				int renderableObjectID = windowRender->addObject(it->first);
			}
			if (i == 1) {
				ImGui::Text("");
				ImGui::Separator();
				ImGui::Text("");
			}
			if (i++ % 2 == 0) ImGui::SameLine();
		}
		it++;
	}
}

/**
 * @brief Car list.
*/
void GUI::carList(Point* point) {
	size_t i = 0;
	std::map<int, Object3Dvehicle>::iterator it = objectStorage->object3Dvehicles.begin();
	while (it != objectStorage->object3Dvehicles.end()) {
		if (it->second.getType() == "vehicle") {
			ImColor color(255, 255, 255, 255);
			bool selected = true;
			if (point->startConfiguration[i] != it->first) {
				color = ImColor(255, 255, 255, 100);
				selected = false;
			}
			if (ImGui::ImageButton((void*)(intptr_t)it->second.getIcon(), ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), -1, color)) {
				if (selected) {
					point->startConfiguration[i] = -1;
				} else {
					point->startConfiguration[i] = it->first;
				}
			}
			if (i++ % 2 == 0) ImGui::SameLine();
		}
		it++;
	}
}

/**
 * @brief Endpoint selector generator for start point properties.
 * @param startPointID The current start point.
*/
void GUI::endpointSelector(Point* point) {
	std::vector<size_t> endpoints = animator->getGraph()->getEndPoints();
	std::string labelText = "Endpoint: ";
	for (size_t i = 0; i < endpoints.size(); i++)	{
		std::string s = std::to_string(endpoints[i]);
		bool currentCheckBox = point->endpointsList[i];
		ImGui::Checkbox((labelText + s).c_str(), &currentCheckBox);
		point->endpointsList[i] = currentCheckBox;
	}
}

/**
 * @brief The main GUI function. Set the GUI structure.
*/
void GUI::draw() {
	if(debugMode) ImGui::ShowTestWindow();

	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoMove;
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoResize;
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Traffic Simulation", NULL,  trafficSimulationWindowFlag)) {
		mainMenuBar();
		windowHandler();

		static int easterEgg = 0;
		if (easterEgg < 10) {
			if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("app_logo.png"), ImVec2(70, 70), ImVec2(0, 0), ImVec2(1, 1), -1, ImColor(255, 255, 255, 0))) { easterEgg++; }
		} else {
			if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("bundus.png"), ImVec2(70, 70))) { easterEgg = 0; }
		}

		ImGui::SameLine();
		ImGui::Text("Traffic Simulation \nBy: Sandor Balazs \nAZA6NL");

		if (!editorLock) {
			if (ImGui::Button("-        Finalizing map        -")) {
				finalise();
			}
			if (finalisingErrorWindow) {
				ImGui::OpenPopup("Not enough start or endpoint.");
				if (ImGui::BeginPopupModal("Not enough start or endpoint.")) {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "You must place one or more start and endpoint \nand stick thees to the roads.");
					if (ImGui::Button("Close")) {
						finalisingErrorWindow = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
		}

		if (editorLock) {
			if (!simulationStart) {
				if (ImGui::Button("-   Edit mode   -")) {
					backToEditMode();
				}
				ImGui::SameLine();
			}
			if (!simulationStart) {
				if (ImGui::Button("-   Start   -")) {
					startSimulation();
				}
			} 
			if (simulationStart) {
				if (!simulationPaused) {
					if (ImGui::Button("-   Pause    -")) {
						pauseSimulation();
					}
				} else {
					if (ImGui::Button("-  Continue  -")) {
						startSimulation();
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("-     STOP     -")) {
					stopSimulation();
				}
			}
		}
	}
	ImGui::End();

	if (mapEditorWindow) {
		mapEditorWindowFlag |= ImGuiWindowFlags_NoMove;
		if (ImGui::Begin("Map editor", NULL, mapEditorWindowFlag)) {
			ImGui::Text("Edit map and add new items:");
			ImGui::Separator();
			ImGui::Text("");

			if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures/road_mini.png"), ImVec2(42, 42))) {
				int newRoadID = windowRender->addRoad();
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures/delete_mini.png"), ImVec2(42, 42))) {
				workingWindow->getEventListener()->deleteSelectedItems();
			}
			itemList();
		}
		ImGui::End();
	}

	if (simulationWindow) {
		simulationWindowFlag |= ImGuiWindowFlags_NoMove;
		if (ImGui::Begin("Simulation", NULL, simulationWindowFlag)) {

			if (selectedStartPoint == -1 && selectedEndPoint == -1 && selectedRoad == -1 && selectedVehicle == -1) {
				ImGui::Separator();
				ImGui::Text("        "); ImGui::SameLine();
				ImGui::Image((void*)(intptr_t)objectStorage->getTexture("miniatures/desk_square_mini.png"), ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Text("");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Select the start points \nfor configuration.");
				ImGui::Text("");
				ImGui::Separator();
				ImGui::Text("");
				ImGui::Text("You can select and observe \nthe startpoints, endpoints, \nthe roads and the vehicles \n with right click anytime.");
				ImGui::Text("");
				ImGui::Separator();
				ImGui::Text("");
				if (!simulationStart) {
					ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "When you ready \nclick to START.");
				} else {
					ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "You can STOP or PAUSE \nthe simulation anytime.");
				}
				ImGui::Text("");
				ImGui::Separator();
			}

			if (this->selectedEndPoint != -1) {
				ImGui::Separator();
				ImGui::Image((void*)(intptr_t)objectStorage->getTexture("miniatures/stop_sign_mini.png"), ImVec2(50, 50));
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "\nSelected endpoint ID: %i.", animator->getGraph()->getPoint(this->selectedEndPoint)->getID());
				ImGui::Separator();
				ImGui::Text("");
				ImGui::Text("Endpoint statistics:");
				ImGui::Text("Recived vehicles count: %i.", animator->getGraph()->getPoint(this->selectedEndPoint)->receivedVehicles);
				ImGui::Text("");
				ImGui::Separator();
			}

			if (this->selectedStartPoint != -1) {
				Point* point = animator->getGraph()->getPointByID(animator->getGraph()->getPoint(this->selectedStartPoint)->getID());
				ImGui::Separator();
				ImGui::Image((void*)(intptr_t)objectStorage->getTexture("miniatures/start_sign_mini.png"), ImVec2(50, 50));
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "\nThe selected \nstartpoint: %i.", point->getID());
				ImGui::Separator();
				ImGui::Text("");
				if (ImGui::Button(" - Start a vehicle from here - ")) {
					animator->addVehicle(point->getID(), true);
				}
				ImGui::Text("");
				ImGui::Separator();
				ImGui::Text("");
					ImGui::Text("Startable vehicles number:");
				ImGui::SliderInt("", &point->startableVehicles, 1, 250);
				ImGui::Text("");
				ImGui::Separator();
				ImGui::Text("Select the target endpoints \nfrom this startpoint:");
				ImGui::Text("");
				endpointSelector(point);
				ImGui::Text("");
				ImGui::Separator();
				ImGui::Text("Select the startable vehicle types \n from this startpoint:");
				ImGui::Text("");
				carList(point);
				ImGui::Text("");
				ImGui::Separator();
			}

			if (this->selectedRoad != -1) {
				ImGui::Separator();
				ImGui::Text("");
				ImGui::Text("   "); ImGui::SameLine();
				ImGui::Image((void*)(intptr_t)objectStorage->getTexture("miniatures/road_mini.png"), ImVec2(50, 50));
				ImGui::SameLine();
				size_t renderID = this->animator->getGraph()->getEdge(this->selectedRoad)->getRoad3DiD();
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Selected road \nrender ID: %i.\nSelected road \nmodel ID:  %i.", renderID, this->selectedRoad);
				ImGui::Text("");
				ImGui::Text("Road statistics:");
				ImGui::Separator();
				ImGui::Text("Road coast:...........%i", this->animator->getGraph()->getEdge(this->selectedRoad)->getCoast());
				ImGui::Text("Road vehicle coast:...%i", this->animator->getGraph()->getEdge(this->selectedRoad)->getVehicleCoast());
				ImGui::Text("Road length:..........%i", this->animator->getGraph()->getEdge(this->selectedRoad)->getLength());
				ImGui::Text("Current vehicles:.....%i", this->animator->getGraph()->getEdge(this->selectedRoad)->getVehicleCount());
				ImGui::Text("All vehicles:.........%i", this->animator->getGraph()->getEdge(this->selectedRoad)->getAllVehicleCount());
				ImGui::Text("");
				ImGui::Separator();
			}

			if (this->selectedVehicle != -1) {
				ImGui::Separator();
				size_t renderID = this->selectedVehicle;
				size_t modelID = this->windowRender->getVehicle(renderID)->getModelID();
				ImGui::Image((void*)(intptr_t)this->windowRender->getVehicle(renderID)->getIcon(), ImVec2(100, 100));
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "\n\nSelected vehicle \nrender ID: %i.\nSelected vehicle \nmodel ID: %i.", renderID, modelID);
				if (animator->getVehicleModel(modelID)->isBlocked()) {
					ImGui::Text("");
					ImGui::Text("Vehicle is blocked \nby another vehicle!");
					ImGui::SameLine();
					if (ImGui::Button("FREE")) {
						animator->getVehicleModel(modelID)->unblock();
					}
					ImGui::Text("");
				}
				ImGui::Text("Vehicle statistics:");
				ImGui::Separator();
				ImGui::Text("Start point:............%i", animator->getVehicleModel(modelID)->startID);
				ImGui::Text("Destination point:......%i", animator->getVehicleModel(modelID)->destinationID);
				ImGui::Text("Path count:.............%i", animator->getVehicleModel(modelID)->hopCounter);
				ImGui::Text("Vehicle Weight:.........%i", animator->getVehicleModel(modelID)->vehicleWeight);
				ImGui::Text("All coast:..............%i", animator->getVehicleModel(modelID)->allCost);
				ImGui::Text("Start time (sec):.......%i", animator->getVehicleModel(modelID)->startTime / 1000);
				ImGui::Text("Current time (sec):.....%i", SDL_GetTicks() / 1000);
				ImGui::Text("");
				ImGui::Separator();
			}
		}
		ImGui::End();
	}
}

/**
 * @brief Render the GUI graphics in the render cycle.
*/
void GUI::render() {
	ImGui_ImplSdlGL3_NewFrame(window);
	draw();
	ImGui::Render();
}

/**
 * @brief Application closing checker window.
 * @return The user want to close?
*/
void GUI::close() {
	closingCheckerWindowStatus = true;
}

/**
 * @brief Show a help when hovered.
 * @param desc The help text.
*/
void GUI::showHelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

/**
 * @brief Show the selected endpoint info.
 * @param pointModelID The selected point. (Model object!)
*/
void GUI::showEndpointInfo(size_t pointModelID) {
	if (pointModelID != -1) {
		if (this->windowRender->getAnimator()->getGraph()->getPoint(pointModelID)->isStartPoint()) this->selectedStartPoint = pointModelID;
		if (this->windowRender->getAnimator()->getGraph()->getPoint(pointModelID)->isEndPoint()) this->selectedEndPoint = pointModelID;
	}
}

/**
 * @brief Show the selected road info.
 * @param edgeModelID The selected edge. (Model object!)
*/
void GUI::showRoadInfo(size_t edgeModelID) {
	if (edgeModelID != -1) {
		this->selectedRoad = edgeModelID;
	}
}

/**
 * @brief Show the selected vehicle data.
 * @param vehicleModelID The selected vehicle. (Model object!)
*/
void GUI::showVehicleInfo(size_t vehicleModelID) {
	if (vehicleModelID != -1) {
		this->selectedVehicle = vehicleModelID;
	}
}

/**
 * @brief Reset the selected item info window content.
*/
void GUI::resetInfoWindow() {
	this->selectedStartPoint = -1;
	this->selectedEndPoint = -1;
	this->selectedRoad = -1;
	this->selectedVehicle = -1;
}

/**
 * @brief Close all windows.
*/
void GUI::closeAllWindows(std::string exeption) {
	if(exeption != "openWindowStatus") openWindowStatus = false;
	if(exeption != "saveWindowStatus") saveWindowStatus = false;
	if(exeption != "saveAsWindowStatus") saveAsWindowStatus = false;
	if(exeption != "ImGuiSettingsWindowStatus") ImGuiSettingsWindowStatus = false;
	if(exeption != "newMapConfirmWindowStatus") newMapConfirmWindowStatus = false;
	if(exeption != "controlsWindowStatus") controlsWindowStatus = false;
	if(exeption != "aboutWindowStatus") aboutWindowStatus = false;
	if(exeption != "closingCheckerWindowStatus") closingCheckerWindowStatus = false;
	if(exeption != "graphicSettingsWindowStatus") graphicSettingsWindowStatus = false;
	if(exeption != "debugOptionsWindowStatus") debugOptionsWindowStatus = false;
	if(exeption != "runningStatisticsWindowStatus") runningStatisticsWindowStatus = false;
	if(exeption != "pathFinderTestWindowStatus") pathFinderTestWindowStatus = false;
	if(exeption != "simulationSettingsWindowStatus") simulationSettingsWindowStatus = false;
	if(exeption != "simulationStatisticsWindowStatus") simulationStatisticsWindowStatus = false;
	if(exeption != "finalisingErrorWindow") finalisingErrorWindow = false;
	if(exeption != "helpWindowStatus") helpWindowStatus = false;
}

/**
 * @brief QuickSave function.
*/
void GUI::quickSave() {
	this->saveWindowStatus = true;
}