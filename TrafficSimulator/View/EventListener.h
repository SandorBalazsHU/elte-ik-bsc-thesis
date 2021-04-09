#pragma once
#include <SDL.h>

class EventListener {
public:
	EventListener(void);
	~EventListener(void);

	void keyboardDown(SDL_KeyboardEvent&);
	void keyboardUp(SDL_KeyboardEvent&);
	void mouseMove(SDL_MouseMotionEvent&);
	void mouseDown(SDL_MouseButtonEvent&);
	void mouseUp(SDL_MouseButtonEvent&);
	void mouseWheel(SDL_MouseWheelEvent&);
	void resize(int, int);
private:

};