#pragma once
#include <SDL.h>
#include "WorkWindow.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"

class GUI {
public:
	GUI(void);
	~GUI(void);
	static bool init(SDL_Window* window);
	static void clean();
	void bind(WorkWindow* currentWindow);
	void guiDraw();
	void render(SDL_Window* window);
	void eventHandler(SDL_Event& event);
	bool isKeyboardCaptured();
	bool isMouseCaptured();

private:
	WorkWindow* workingWindow;
	SDL_Window* window;
};