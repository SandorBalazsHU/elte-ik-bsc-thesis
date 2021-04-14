#pragma once
#include <SDL.h>
#include "Camera.h"

class EventListener {
public:
	EventListener(void);
	~EventListener(void);
	void init(Camera camera);
	bool event(SDL_Event event);

private:
	Camera camera;
	void keyboardDown(SDL_KeyboardEvent&);
	void keyboardUp(SDL_KeyboardEvent&);
	void mouseMove(SDL_MouseMotionEvent&);
	void mouseDown(SDL_MouseButtonEvent&);
	void mouseUp(SDL_MouseButtonEvent&);
	void mouseWheel(SDL_MouseWheelEvent&);
	void resize(int, int);
};