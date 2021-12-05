//Spirit In The Sky
#pragma once

class Render;
class Vehicle;

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
	void finalize();
	void start();
	void pause();
	void stop();
	size_t addVehicle(size_t type);
	Graph* getGraph();

private:
	Render* render = NULL;
	Graph* graph = NULL;
	void timerUpdate();
	Uint32 currentTime = 0;
	Uint32 prevouseTime = 0;
	bool isAnimationRunning = false;
	size_t currentIndex = 0;

	int startableCarsNumber = 40;
	std::vector<Vehicle> vehicles;
	std::vector<size_t> startPoints;
	std::vector<size_t> endPoints;
};