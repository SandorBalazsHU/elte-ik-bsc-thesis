#pragma once
#include <SDL.h>

class fpsCounter {
public:
	static void bind();
	static void start();
	static void stop();
	static void fpsLimiterIsOn(bool isOn) {
		isLimiterOn = isOn;
	}
	static void setFpsLimit(float limit) {
		fpsLimit = limit;
	}
private:
	fpsCounter();
	~fpsCounter();
	static float fpsLog[100];
	static int fpsLogCounter;
	static float currentFPS;
	static float averageFPS;
	static float fpsLimit;
	static bool isLimiterOn;
	static Uint32 limiterCurrentTime;
	static Uint32 currentTime;
	static Uint32 previousTime;
	static SDL_atomic_t atomicFrameCounter;
	static void fpsLimiter(float limit);
	static Uint32 fpsTimerCallback(Uint32 interval, void* data);
};