#include "EventListener.h"
#include "WorkWindow.h"
#include "GUI.h"

EventListener::EventListener(void) {
}

EventListener::~EventListener(void) {
}

void EventListener::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	camera = workingWindow->getCamera();
	gui = workingWindow->getGUI();
}

void EventListener::keyboardDown(SDL_KeyboardEvent& key) {
	camera->keyboardDown(key);
}

void EventListener::keyboardUp(SDL_KeyboardEvent& key) {
	camera->keyboardUp(key);
}

void EventListener::mouseMove(SDL_MouseMotionEvent& mouse) {
	camera->mouseMove(mouse);
}

void EventListener::mouseDown(SDL_MouseButtonEvent& mouse) {}

void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {}

void EventListener::mouseWheel(SDL_MouseWheelEvent& wheel) {
	camera->mouseWheel(wheel);
}

void EventListener::resize(SDL_WindowEvent& window) {
	int with = window.data1;
	int height = window.data2;
	camera->resize(with, height);
}


void EventListener::exit() {
	workingWindow->close();
}

void EventListener::eventProcessor() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		gui->eventHandler(&event);

		if (!gui->isMouseCaptured() && !gui->isKeyboardCaptured()) {
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
	}
}