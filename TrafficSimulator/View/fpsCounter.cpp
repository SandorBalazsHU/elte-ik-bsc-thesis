#include "fpsCounter.h"

fpsCounter::fpsCounter() {}

fpsCounter::~fpsCounter() {}

float fpsCounter::fpsLog[100] = { 0.0f };

int fpsCounter::fpsLogCounter = 0;

float fpsCounter::fpsLimit = 60.0f;

bool fpsCounter::isLimiterOn = false;

float fpsCounter::currentFPS;
float fpsCounter::averageFPS;
Uint32 fpsCounter::limiterCurrentTime;
Uint32 fpsCounter::currentTime;
Uint32 fpsCounter::previousTime;
SDL_atomic_t fpsCounter::atomicFrameCounter;

void fpsCounter::bind() {
	SDL_AddTimer(averageFPSupdateTime, &fpsCounter::fpsTimerCallback, NULL);
}

void fpsCounter::start() {
	fpsLogCounter++;
	if (fpsLogCounter == 100) fpsLogCounter = 0;
	previousTime = SDL_GetTicks();
}

void fpsCounter::stop() {
	limiterCurrentTime = SDL_GetTicks();
	SDL_AtomicAdd(&atomicFrameCounter, 1);
	if(isLimiterOn) fpsLimiter(fpsLimit);
	currentTime = SDL_GetTicks();
	currentFPS = 1000.0f / (currentTime - previousTime);
	fpsLog[fpsLogCounter] = currentFPS;
}

void fpsCounter::fpsLimiter(float limit) {
	float screenTicksPerFrame = 1000.0f / limit;
	float frameTicks = limiterCurrentTime - previousTime;
	if (frameTicks < screenTicksPerFrame) {
		SDL_Delay(screenTicksPerFrame - frameTicks);
	}
}

Uint32 fpsCounter::fpsTimerCallback(Uint32 interval, void* data) {
	averageFPS = SDL_AtomicGet(&atomicFrameCounter) / (interval * 0.001f);
	// Reset frame counter
	SDL_AtomicSet(&atomicFrameCounter, 0);
	return interval;
}