#pragma once

class Render;

#include<vector>
#include<set>
#include<SDL.h>

class Animator {
public:
	Animator(void);
	~Animator(void);
	void bind(Render* render);
	void update();
	void start();
	void stop();

private:
	Render* render;
	void timerUpdate();
	Uint32 currentTime;
	Uint32 prevouseTime;
	bool isAnimationRunning = false;
	size_t currentIndex = 0;
};