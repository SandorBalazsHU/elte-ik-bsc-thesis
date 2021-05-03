/*#pragma once
#include <SDL.h>
#include "WorkWindow.h"

class EventListener {
public:
	EventListener(WorkWindow*);
	~EventListener(void);
	void event(SDL_Event);

private:
	WorkWindow* window;
	Camera* camera;
	void keyboardDown(SDL_KeyboardEvent&);
	void keyboardUp(SDL_KeyboardEvent&);
	void mouseMove(SDL_MouseMotionEvent&);
	void mouseDown(SDL_MouseButtonEvent&);
	void mouseUp(SDL_MouseButtonEvent&);
	void mouseWheel(SDL_MouseWheelEvent&);
	void resize(SDL_WindowEvent& window);
	void exit();
};*/