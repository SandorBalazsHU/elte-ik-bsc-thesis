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
	return ImGui_ImplSdlGL3_Init(window);
	/*ImGuiStyle& style = ImGui::GetStyle();
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.FrameRounding = 3;
	style.FrameBorderSize = 1;*/
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
			//ShowExampleMenuFile();
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
				eventListener->select(renderableObjectID);
			}
			if (i++ % 2 == 0) ImGui::SameLine();
		}
		it++;
	}
}


void GUI::draw() {
	mainMenuBar();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 240), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Traffic Simulation")) {
		//ImGui::Text("Traffic Simulation");
		ImGui::Text("By: Sandor Balazs - AZA6NL");
		//loadingBar();
		//fpsGraph();
		//styleTab();
		itemList();
	}
	ImGui::End();
}

void GUI::render() {
	ImGui_ImplSdlGL3_NewFrame(window);
	draw();
	ImGui::Render();
}