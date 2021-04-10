#include <SDL_opengl.h>
#include "EventListener.h"

EventListener::EventListener(void) {
}

EventListener::~EventListener(void) {
}

void EventListener::init(Camera camera) {
	this->camera = camera;
}

void EventListener::keyboardDown(SDL_KeyboardEvent& key) {
	camera.KeyboardDown(key);
}

void EventListener::keyboardUp(SDL_KeyboardEvent& key) {
	camera.KeyboardUp(key);
}

void EventListener::mouseMove(SDL_MouseMotionEvent& mouse) {
	camera.MouseMove(mouse);
}

void EventListener::mouseDown(SDL_MouseButtonEvent& mouse) {}
void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {}

void EventListener::mouseWheel(SDL_MouseWheelEvent& wheel) {
	camera.MouseWheel(wheel);
}

void EventListener::resize(int _w, int _h) {
	glViewport(0, 0, _w, _h);
	camera.Resize(_w, _h);
}

bool EventListener::event(SDL_Event event) {
	bool quit = false;
	switch (event.type)
	{
	case SDL_QUIT:
		quit = true;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
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
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			resize(event.window.data1, event.window.data2);
		break;
	}
	return quit;
}