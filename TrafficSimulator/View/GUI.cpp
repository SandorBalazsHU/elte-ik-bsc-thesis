#include "GUI.h"

GUI::GUI(void) {

}
GUI::~GUI(void) {

}

void GUI::init(SDL_Window* window) {
	ImGui_ImplSdlGL3_Init(window);
}