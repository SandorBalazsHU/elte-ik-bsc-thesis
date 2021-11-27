//Spirit In The Sky
#pragma once

class Render;

#include<vector>
#include<set>
#include<SDL.h>
#include "../Model/Graph.h"

class Animator {
public:
	Animator(void);
	~Animator(void);
	void bind(Render* render);
	void update();
	void start();
	void stop();

private:
	Render* render = NULL;
	Graph* graph = NULL;
	void timerUpdate();
	Uint32 currentTime = 0;
	Uint32 prevouseTime = 0;
	bool isAnimationRunning = false;
	size_t currentIndex = 0;

	int startableCarsNumber = 40;
	std::vector<size_t> startableCars;
};