/*#include "EventListener.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

EventListener::EventListener(WorkWindow* window) : window(window), camera(window->getCamera()) {
}

EventListener::~EventListener(void) {
}

void EventListener::keyboardDown(SDL_KeyboardEvent& key) {
	camera->KeyboardDown(key);
}

void EventListener::keyboardUp(SDL_KeyboardEvent& key) {
	camera->KeyboardUp(key);
}

void EventListener::mouseMove(SDL_MouseMotionEvent& mouse) {
	camera->MouseMove(mouse);
}

void EventListener::mouseDown(SDL_MouseButtonEvent& mouse) {}

void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {}

void EventListener::mouseWheel(SDL_MouseWheelEvent& wheel) {
	camera->MouseWheel(wheel);
}

void EventListener::resize(SDL_WindowEvent& window) {
	int with = window.data1;
	int height = window.data2;
	camera->Resize(with, height);
}


void EventListener::exit() {
	window->close();
}

void EventListener::event(SDL_Event event) {
	ImGui_ImplSdlGL3_ProcessEvent(&event);
	bool isMouseEventCaptured = ImGui::GetIO().WantCaptureMouse;
	bool isKeyEventCaptured = ImGui::GetIO().WantCaptureKeyboard;

	if (!isMouseEventCaptured && !isKeyEventCaptured) {
		switch (event.type) {
		case SDL_QUIT:
			exit();
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) exit();
			keyboardDown(event.key);
			break;
		case SDL_KEYUP:
			keyboardUp(event.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseDown(event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			mouseUp(event.button);
			break;
		case SDL_MOUSEWHEEL:
			mouseWheel(event.wheel);
			break;
		case SDL_MOUSEMOTION:
			mouseMove(event.motion);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) resize(event.window);
			break;
		}
	}
}*/