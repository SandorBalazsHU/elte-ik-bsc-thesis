#include "Render.h"

Render::Render(WorkWindow* window) : window(window) {
}

int Render::render() {
	SDL_Event event;
	//The FPS Counter
	fpsCounter++;
	if (fpsCounter == 100) fpsCounter = 0;
	const Uint32 time = SDL_GetTicks();

	//Event processing loop
	while (SDL_PollEvent(&event)) {
		 eventListener.event(event);
	}

	//Renderinga
	//ImGui_ImplSdlGL3_NewFrame(window);
	//simulation.Update();
	camera.Update();
	//simulation.Render();
	glEnable(GL_CULL_FACE); //Face test ON
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	//ImGui::Render();
	SDL_GL_SwapWindow(window);

	//FPS Counter
	const Uint32 last_time = SDL_GetTicks();
	std::stringstream window_title;
	float fps = 1000.0f / (last_time - time);
	window_title << "Traffic Simulation. FPS: " << fps;
	//simulation.fps[fpsPlotCounter] = fps;
	SDL_SetWindowTitle(window, window_title.str().c_str());
}