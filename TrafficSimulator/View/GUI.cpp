/**
 * @name Traffic Simulation
 * @file GUI.cpp
 * @class GUI
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
 * Contact: sandorbalazs9402@gmail.com
 * KSP
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
			if (ImGui::MenuItem("New", "Ctrl+N", &newMapConfirmWindowStatus)) {}
			if (ImGui::MenuItem("Open", "Ctrl+O", &openWindowStatus)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S", &saveWindowStatus)) {}
			if (ImGui::MenuItem("Save As..", "Ctrl+A", &saveAsWindowStatus)) {}
			if (ImGui::MenuItem("Close", "Ctrl+X")) {
				windowRender->close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Graphics settings", "CTRL+G")) {}
			if (ImGui::MenuItem("ImGui settings", "CTRL+I", &ImGuiSettingsWindowStatus));
			ImGui::Separator();
			if (ImGui::MenuItem("Running statistics", "CTRL+R")) {}
			if (ImGui::MenuItem("Debug options", "CTRL+D")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Simulation"))
		{
			if (ImGui::MenuItem("START Simulation", "CTRL+Q")) {}
			if (ImGui::MenuItem("STOP Simulation", "CTRL+W")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Simulation settings", "CTRL+E")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Simulation statistics", "CTRL+f")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Help", "CTRL+H")) {}
			if (ImGui::MenuItem("Controls", "CTRL+C", &controlsWindowStatus)) ;
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
	if (ImGuiSettingsWindowStatus) { ImGui::Begin("ImGui Style Editor", &ImGuiSettingsWindowStatus); ImGui::ShowStyleEditor(); ImGui::End(); }
}

void GUI::fpsGraph() {
	std::ostringstream ss;
	ss << "Average FPS: " << fpsCounter::getAverageFPS();
	ImGui::Text(ss.str().c_str());
	ImGui::Text("ImGUI average: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::PushItemWidth(-1);
	ImGui::PlotLines("FPS", fpsCounter::fpsLog, IM_ARRAYSIZE(fpsCounter::fpsLog), 0, "FPS", 0.0f, 150.0f, ImVec2(0, 80));
	ImGui::PlotHistogram("FP2S", fpsCounter::fpsLog, IM_ARRAYSIZE(fpsCounter::fpsLog), 0, "FPS2", 0.0f, 150.0f, ImVec2(0, 80));
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

void GUI::draw() {
	ImGui::ShowTestWindow();
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(251, 130), ImGuiCond_FirstUseEver);
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoMove;
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoResize;
	trafficSimulationWindowFlag |= ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Traffic Simulation", NULL,  trafficSimulationWindowFlag)) {
		mainMenuBar();
		windowHandler();

		ImGui::Image((void*)(intptr_t)objectStorage->getTexture("app_logo.png"), ImVec2(70, 70));
		ImGui::SameLine();
		ImGui::Text("Traffic Simulation \nBy: Sandor Balazs \nAZA6NL");

		if (ImGui::Button("- Finalizing map and start! -")) {}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 150), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(252, 643), ImGuiCond_FirstUseEver);
	mapEditorWindowFlag |= ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Map editor", NULL, mapEditorWindowFlag)) {
		//fpsGraph();
		ImGui::Text("Edit map and add new items:");
		ImGui::Separator();
		ImGui::Text("");

		if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures\\road_mini.png"), ImVec2(42, 42))) {
			int newRoadID = windowRender->addRoad();
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures\\delete_mini.png"), ImVec2(42, 42))) {
			//TODO function
		}
		itemList();
	}
	ImGui::End();
}

void GUI::render() {
	ImGui_ImplSdlGL3_NewFrame(window);
	draw();
	ImGui::Render();
}