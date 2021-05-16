#include "GUI.h"

GUI::GUI(void) {

}

GUI::~GUI(void) {

}

bool GUI::init(SDL_Window* window) {
	return ImGui_ImplSdlGL3_Init(window);
}

void GUI::clean() {
	ImGui_ImplSdlGL3_Shutdown();
}

void GUI::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	window = currentWindow->getWindow();
}

void GUI::eventHandler(SDL_Event& event) {
	ImGui_ImplSdlGL3_ProcessEvent(&event);
}

bool GUI::isKeyboardCaptured() {
	return ImGui::GetIO().WantCaptureMouse;
}

bool GUI::isMouseCaptured() {
	return ImGui::GetIO().WantCaptureKeyboard;
}

void GUI::guiDraw() {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 240), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Rigid Body Simulation")) {
		ImGui::Text("Rigid Body Simulation");
		ImGui::Text("By: Sandor Balazs");
	}
	ImGui::End();
}

void GUI::render(SDL_Window* window) {
	guiDraw();
	ImGui_ImplSdlGL3_NewFrame(window);
	ImGui::Render();
}