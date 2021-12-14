/**
 * @name Traffic Simulation
 * @file fpsCounter.h
 * @class fpsCounter
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief FPS counter, logger and limiter.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include <SDL.h>

/**
 * @brief FPS counter, logger and limiter static class.
*/
class fpsCounter {
public:

	/**
	 * @brief Initialize the class.
	*/
	static void bind();

	/**
	 * @brief Start FPS counting.
	*/
	static void start();

	/**
	 * @brief Stop the counter.
	*/
	static void stop();

	/**
	 * @brief FPS Limiter ON.
	*/
	static void fpsLimiterOn();

	/**
	 * @brief FPS Limiter OFF
	*/
	static void fpsLimiterOff();

	/**
	 * @brief Set FPS limiter.
	 * @param limit New FPS limit.
	*/
	static void setFpsLimit(float limit);

	/**
	 * @brief Getter for current FPS.
	 * @return current FPS.
	*/
	static float getCurrentFPS();

	/**
	 * @brief Getter for average FPS.
	 * @return average FPS.
	*/
	static float getAverageFPS();

	/**
	 * @brief The FPS Log array
	*/
	static float fpsLog[100];

private:

	/**
	 * @brief Private constructor for a static class.
	*/
	fpsCounter();

	/**
	 * @brief Private destructor for a static class.
	*/
	~fpsCounter();

	/**
	 * @brief Parallel FPS counter update interval.
	*/
	static const int averageFPSupdateTime = 500;

	/**
	 * @brief The FPS log index.
	*/
	static int fpsLogCounter;

	/**
	 * @brief The current FPS.
	*/
	static float currentFPS;

	/**
	 * @brief The average FPS.
	*/
	static float averageFPS;

	/**
	 * @brief The current FPS limit.
	*/
	static float fpsLimit;

	/**
	 * @brief The FPS limiter switch.
	*/
	static bool isLimiterOn;

	/**
	 * @brief The current time for the limiter delay.
	*/
	static Uint32 limiterCurrentTime;

	/**
	 * @brief The current time.
	*/
	static Uint32 currentTime;

	/**
	 * @brief The previous saved time.
	*/
	static Uint32 previousTime;

	/**
	 * @brief The atomic frame counter.
	*/
	static SDL_atomic_t atomicFrameCounter;

	/**
	 * @brief FPS Limiter setter.
	 * @param limit The new FPS limit.
	*/
	static void fpsLimiter(float limit);

	/**
	 * @brief The atomic frame counter callback function.
	 * @param interval The SDL timer interval.
	 * @param data The SDL Timer data.
	 * @return The SDL Timer return.
	*/
	static Uint32 fpsTimerCallback(Uint32 interval, void* data);
};