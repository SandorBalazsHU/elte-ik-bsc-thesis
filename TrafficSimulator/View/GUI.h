#pragma once
#include <SDL.h>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"

class GUI {
public:
	GUI(void);
	~GUI(void);
	void init(SDL_Window* window);
private:
};