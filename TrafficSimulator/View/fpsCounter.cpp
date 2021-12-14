/**
 * @name Traffic Simulation
 * @file fpsCounter.cpp
 * @class fpsCounter
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief FPS counter, logger and limiter.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "fpsCounter.h"

/**
 * @brief Private constructor for a static class.
*/
fpsCounter::fpsCounter() {}

/**
 * @brief Private destructor for a static class.
*/
fpsCounter::~fpsCounter() {}

/**
 * @brief The FPS Log array
*/
float fpsCounter::fpsLog[100] = { 0.0f };

/**
 * @brief The FPS log index.
*/
int fpsCounter::fpsLogCounter = 0;

/**
 * @brief The current FPS limit.
*/
float fpsCounter::fpsLimit = 60.0f;

/**
 * @brief The FPS limiter switch.
*/
bool fpsCounter::isLimiterOn = false;

/**
 * @brief The current FPS.
*/
float fpsCounter::currentFPS;

/**
 * @brief The average FPS.
*/
float fpsCounter::averageFPS;

/**
 * @brief The current time for the limiter delay.
*/
Uint32 fpsCounter::limiterCurrentTime;

/**
 * @brief The current time.
*/
Uint32 fpsCounter::currentTime;

/**
 * @brief The previous saved time.
*/
Uint32 fpsCounter::previousTime;

/**
 * @brief The atomic frame counter.
*/
SDL_atomic_t fpsCounter::atomicFrameCounter;

/**
 * @brief Initialize the class.
*/
void fpsCounter::bind() {
	SDL_AddTimer(averageFPSupdateTime, &fpsCounter::fpsTimerCallback, NULL);
}

/**
 * @brief Start FPS counting.
*/
void fpsCounter::start() {
	fpsLogCounter++;
	if (fpsLogCounter == 100) fpsLogCounter = 0;
	previousTime = SDL_GetTicks();
}

/**
 * @brief Stop the counter.
*/
void fpsCounter::stop() {
	limiterCurrentTime = SDL_GetTicks();
	SDL_AtomicAdd(&atomicFrameCounter, 1);
	if(isLimiterOn) fpsLimiter(fpsLimit);
	currentTime = SDL_GetTicks();
	currentFPS = 1000.0f / (currentTime - previousTime);
	fpsLog[fpsLogCounter] = currentFPS;
}

/**
 * @brief FPS Limiter setter.
 * @param limit The new FPS limit.
*/
void fpsCounter::fpsLimiter(float limit) {
	float screenTicksPerFrame = 1000.0f / limit;
	float frameTicks = limiterCurrentTime - previousTime;
	if (frameTicks < screenTicksPerFrame) {
		SDL_Delay(screenTicksPerFrame - frameTicks);
	}
}

/**
 * @brief The atomic frame counter callback function.
 * @param interval The SDL timer interval.
 * @param data The SDL Timer data.
 * @return The SDL Timer return.
*/
Uint32 fpsCounter::fpsTimerCallback(Uint32 interval, void* data) {
	averageFPS = SDL_AtomicGet(&atomicFrameCounter) / (interval * 0.001f);
	// Reset frame counter
	SDL_AtomicSet(&atomicFrameCounter, 0);
	return interval;
}

/**
 * @brief FPS Limiter ON.
*/
void fpsCounter::fpsLimiterOn() {
	isLimiterOn = true;
}

/**
 * @brief FPS Limiter OFF
*/
void fpsCounter::fpsLimiterOff() {
	isLimiterOn = false;
}

/**
 * @brief Set FPS limiter.
 * @param limit New FPS limit.
*/
void fpsCounter::setFpsLimit(float limit) {
	fpsLimit = limit;
}

/**
 * @brief Getter for current FPS.
 * @return current FPS.
*/
float fpsCounter::getCurrentFPS() {
	return currentFPS;
}

/**
 * @brief Getter for average FPS.
 * @return average FPS.
*/
float fpsCounter::getAverageFPS() {
	return averageFPS;
}