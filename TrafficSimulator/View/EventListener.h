#pragma once

class WorkWindow;
class GUI;

#include<SDL.h>
#include"Camera.h"


class WorkWindow;

class EventListener {
public:
	EventListener();
	~EventListener(void);
	void bind(WorkWindow* currentWindow);
	void eventProcessor();
private:
	WorkWindow* workingWindow;
	Camera* camera;
	GUI* gui;
	void keyboardDown(SDL_KeyboardEvent&);
	void keyboardUp(SDL_KeyboardEvent&);
	void mouseMove(SDL_MouseMotionEvent&);
	void mouseDown(SDL_MouseButtonEvent&);
	void mouseUp(SDL_MouseButtonEvent&);
	void mouseWheel(SDL_MouseWheelEvent&);
	void resize(SDL_WindowEvent& window);
	void exit();
};