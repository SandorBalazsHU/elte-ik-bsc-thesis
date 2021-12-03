/**
 * @name Traffic Simulation
 * @file GUI.cpp
 * @class GUI
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
 * Contact: sandorbalazs9402@gmail.com
 * KSP
 * alarm 1202
 * sce to aux
*/

#include "GUI.h"
#include "WorkWindow.h"
#include "Render.h"
#include "EventListener.h"
#include "fpsCounter.h"
#include <sstream>

GUI::GUI(void) {

}

GUI::~GUI(void) {

}

bool GUI::init(SDL_Window* window) {
	ImGuiStyle& style = ImGui::GetStyle();
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.FrameRounding = 3;
	style.FrameBorderSize = 1;
	return ImGui_ImplSdlGL3_Init(window);
}

void GUI::clean() {
	ImGui_ImplSdlGL3_Shutdown();
}

void GUI::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	window = currentWindow->getWindow();
	objectStorage = workingWindow->getObjectStorage();
	windowRender = currentWindow->getRender();
	eventListener = currentWindow->getEventListener();
	animator = windowRender->getAnimator();
}

void GUI::eventHandler(SDL_Event* event) {
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

bool GUI::isKeyboardCaptured() {
	return ImGui::GetIO().WantCaptureMouse;
}

bool GUI::isMouseCaptured() {
	return ImGui::GetIO().WantCaptureKeyboard;
}

void GUI::mainMenuBar() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N", &newMapConfirmWindowStatus, !editorLock)) {}
			if (ImGui::MenuItem("Open", "Ctrl+O", &openWindowStatus, !editorLock)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S", &saveWindowStatus)) {}
			if (ImGui::MenuItem("Save As..", "Ctrl+A", &saveAsWindowStatus)) {}
			if (ImGui::MenuItem("Close", "Ctrl+X", &closingCheckerWindowStatus)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Graphics settings", "CTRL+G", &graphicSettingsWindowStatus)) {}
			if (ImGui::MenuItem("ImGui settings", "CTRL+I", &ImGuiSettingsWindowStatus)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Running statistics", "CTRL+R", &runningStatisticsWindowStatus)) {}
			if (ImGui::MenuItem("Debug options", "CTRL+D", &debugOptionsWindowStatus)) {}
			if (ImGui::MenuItem("Pathfinder algorithm test.", "CTRL+P", &pathFinderTestWindowStatus, !editorLock)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Simulation"))
		{
			if (ImGui::MenuItem("Finalizing", "CTRL+Q")) {}
			if (ImGui::MenuItem("START Simulation", "CTRL+Q", editorLock, editorLock)) {}
			if (ImGui::MenuItem("STOP Simulation", "CTRL+W", editorLock, editorLock)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Simulation settings", "CTRL+E")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Simulation statistics", "CTRL+f", editorLock, editorLock)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Help", "CTRL+H")) {}
			if (ImGui::MenuItem("Controls", "CTRL+C", &controlsWindowStatus)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("About", "CTRL+K", &aboutWindowStatus)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

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
	if (ImGuiSettingsWindowStatus) { ImGui::Begin("ImGui Style Editor", &ImGuiSettingsWindowStatus); ImGui::ShowStyleEditor(); ImGui::End(); }
}

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
		//std::cout << it->first << std::endl;
		if (it->second.getType() == "object") {
		//if (true) {
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
void GUI::carList() {
	int i = 0;
	std::map<int, Object3Dvehicle>::iterator it = objectStorage->object3Dvehicles.begin();
	while (it != objectStorage->object3Dvehicles.end()) {
		if (it->second.getType() == "vehicle") {
			if (ImGui::ImageButton((void*)(intptr_t)it->second.getIcon(), ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), -1, ImColor(255, 255, 255, 100))) {
				//int renderableObjectID = windowRender->addObject(it->first);
				int renderableObjectID = windowRender->addVehicle(it->first);
			}
			if (i++ % 2 == 0) ImGui::SameLine();
		}
		it++;
	}
}

void GUI::draw() {
	ImGui::ShowTestWindow();

	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoMove;
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoResize;
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Traffic Simulation", NULL,  trafficSimulationWindowFlag)) {
		mainMenuBar();
		windowHandler();

		ImGui::Image((void*)(intptr_t)objectStorage->getTexture("app_logo.png"), ImVec2(70, 70));
		ImGui::SameLine();
		ImGui::Text("Traffic Simulation \nBy: Sandor Balazs \nAZA6NL");

		if (!editorLock) {
			if (ImGui::Button("-        Finalizing map        -")) {
				animator->finalize();
				if (animator->getGraph()->getEndPoints().size() > 0 && animator->getGraph()->getStartPoints().size() > 0) {
					editorLock = true;
					windowRender->lockEditor();
					mapEditorWindow = false;
					simulationWindow = true;
					animator->finalize();
				} else {
					finalisingErrorWindow = true;
				}
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
			if (ImGui::Button("- Back to edit mode -")) {
				editorLock = false;
				windowRender->freeEditor();
				mapEditorWindow = true;
				simulationWindow = false;
			}
			ImGui::SameLine();
			//TODO Pause and stop 
			if (ImGui::Button("- START -")) {
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

			if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures\\road_mini.png"), ImVec2(42, 42))) {
				int newRoadID = windowRender->addRoad();
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures\\delete_mini.png"), ImVec2(42, 42))) {
				workingWindow->getEventListener()->deleteSelectedItems();
			}
			itemList();
		}
		ImGui::End();
	}

	if (simulationWindow) {
		simulationWindowFlag |= ImGuiWindowFlags_NoMove;
		if (ImGui::Begin("Simulation", NULL, simulationWindowFlag)) {

			ImGui::Text("TEST");
			ImGui::Separator();
			ImGui::Text("");

			carList();

		}
		ImGui::End();
	}
}

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
 * @brief Show a help when howered.
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
	std::cout << "ENDPOINT INFO: " << pointModelID << std::endl;
}

/**
 * @brief Show the selected road info.
 * @param edgeModelID The selected edge. (Model object!)
*/
void GUI::showRoadInfo(size_t edgeModelID) {
	std::cout << "EDGE INFO: " << edgeModelID << std::endl;
}

/**
 * @brief Show the selected vehicle datas.
 * @param vehicleModelID The selected vehicle. (Model object!)
*/
void GUI::showVehicleInfo(size_t vehicleModelID) {
	std::cout << "VEHICLE INFO: " << vehicleModelID << std::endl;
}