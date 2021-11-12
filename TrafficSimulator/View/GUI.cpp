/**
 * @name Traffic Simulation
 * @file Camera.cpp
 * @class TrafficSimulator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Main file.
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

void GUI::styleTab() {
	ImGuiStyle& style = ImGui::GetStyle();

	static int style_idx = 0;
	if (ImGui::Combo("Theme selector", &style_idx, "Classic\0Dark\0Light\0"))
	{
		switch (style_idx)
		{
		case 0: ImGui::StyleColorsClassic(); break;
		case 1: ImGui::StyleColorsDark(); break;
		case 2: ImGui::StyleColorsLight(); break;
		}
	}

	// Simplified Settings
	if (ImGui::SliderFloat("Frame rounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
		style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	{ bool window_border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &window_border)) style.WindowBorderSize = window_border ? 1.0f : 0.0f; }
	ImGui::SameLine();
	{ bool frame_border = (style.FrameBorderSize > 0.0f); if (ImGui::Checkbox("FrameBorder", &frame_border)) style.FrameBorderSize = frame_border ? 1.0f : 0.0f; }
	ImGui::SameLine();
	{ bool popup_border = (style.PopupBorderSize > 0.0f); if (ImGui::Checkbox("PopupBorder", &popup_border)) style.PopupBorderSize = popup_border ? 1.0f : 0.0f; }

}

void GUI::mainMenuBar() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O", &openWindow)) {
				
			}

			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						//ShowExampleMenuFile();
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			ImGui::Separator();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void GUI::fpsGraph() {
	std::ostringstream ss;
	ss << "Average FPS: " << fpsCounter::getAverageFPS();
	ImGui::Text(ss.str().c_str());
	ImGui::PlotLines("FPS", fpsCounter::fpsLog, IM_ARRAYSIZE(fpsCounter::fpsLog), 0, "FPS", 0.0f, 100.0f, ImVec2(0, 80));
}

void GUI::loadingBar() {
;
	float percent = ((float) objectStorage->getLoadingState()) / ((float)objectStorage->getLoadingStateMax());

	/*std::ostringstream ss;
	ss << "Loading: " << objectStorage->getLoadingState() << "/" << objectStorage->getLoadingStateMax() << " " << percent;
	ImGui::Text(ss.str().c_str());*/

	ImGui::ProgressBar(percent, ImVec2(0.0f, 0.0f));
}


static void addNewItemWindow(bool* p_open) {
	/*ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Example: Layout", p_open, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Close")) *p_open = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// left
		static int selected = 0;
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		for (int i = 0; i < 100; i++) {
			char label[128];
			sprintf(label, "MyObject %d", i);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}
		ImGui::EndChild();
		ImGui::SameLine();

		// right
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Text("MyObject: %d", selected);
		ImGui::Separator();
		ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
		ImGui::EndChild();
		ImGui::BeginChild("buttons");
		if (ImGui::Button("Revert")) {}
		ImGui::SameLine();
		if (ImGui::Button("Save")) {}
		ImGui::EndChild();
		ImGui::EndGroup();
	}
	ImGui::End();*/
}

void GUI::itemList() {
	int i = 0;
	std::map<int, Object3D>::iterator it = objectStorage->object3Ds.begin();
	while (it != objectStorage->object3Ds.end()) {
		if (it->second.getType() == "object") {
			if (ImGui::ImageButton((void*)(intptr_t)it->second.getIcon(), ImVec2(100, 100))) {
				int renderableObjectID = windowRender->addObject(it->first);
				//eventListener->select(renderableObjectID);
			}
			if (i++ % 2 == 0) ImGui::SameLine();
		}
		it++;
	}
}


void GUI::draw() {
	ImGui::ShowTestWindow();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 240), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Traffic Simulation")) {
		mainMenuBar();
		//ImGui::Text("Traffic Simulation");
		ImGui::Text("By: Sandor Balazs - AZA6NL");
		//loadingBar();
		//fpsGraph();
		//styleTab();

		if (openWindow) {
			ImGui::SetNextWindowSize(ImVec2(400, 300));
			ImGui::OpenPopup("Open Map");
			if (ImGui::BeginPopupModal("Open Map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Saved maps:");

				ImGui::Separator();

				std::vector<std::string> fileList = windowRender->getMapLoader()->listFiles();
				const int itemNumber = fileList.size();

				const char* items[50];

				for (size_t i = 0; i < itemNumber; i++) {
					items[i] = fileList[i].c_str();
				}

				static int selection = 0;
				ImGui::PushItemWidth(-1);
				ImGui::PushID(0);
				ImGui::ListBox("", &selection, items, itemNumber);
				ImGui::PopID();

				if (ImGui::Button("Open")) {
					windowRender->getMapLoader()->loadMap(items[selection]);
					openWindow = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete")) {
					int status = windowRender->getMapLoader()->deleteSave(items[selection]);
					if (status == 0) ImGui::OpenPopup("Map deleted");
					if(status == 1) ImGui::OpenPopup("Map not found");
					if(status == 2) ImGui::OpenPopup("Map open file system error");
				}
				if (ImGui::BeginPopupModal("Map deleted")) {
					ImGui::Text("Map deleted.");
					if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Map not found")) {
					ImGui::Text("Map not found.");
					if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Map open file system error")) {
					ImGui::Text("File system error.");
					if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close")) {
					openWindow = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures/road_mini.png"), ImVec2(100, 100))) {
			int newRoadID = windowRender->addRoad();
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures/delete_mini.png"), ImVec2(100, 100))) {
			//TODO function
		}
		if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures/large_car_green_mini.png"), ImVec2(100, 100))) {
			//TODO function
			windowRender->getMapSaver()->saveMap("testSave.csv");
			std::cout << "SAVED" << std::endl;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)objectStorage->getTexture("miniatures/large_car_blue_mini.png"), ImVec2(100, 100))) {
			//TODO function
			windowRender->getMapLoader()->loadMap("testSave.csv");
			std::cout << "LOADED" << std::endl;
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