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
	void addVehicle(size_t startPoint = -1, size_t endPoint = -1);
	Graph* getGraph();
	void clear();

private:
	Render* render = NULL;
	Graph* graph = NULL;
	Uint32 currentTime = 0;
	Uint32 prevouseTime = 0;
	Uint32 vehicleStarterCurrentTime = 0;
	Uint32 vehicleStarterPrevouseTime = 0;
	bool isAnimationRunning = false;
	bool softRunning = false;
	int updateFrequency = 20;
	int vehicleStarterUpdateFrequency = 1000;
	int startableCarsNumber = 40;
	std::vector<Vehicle> vehicles;
	std::vector<size_t> startPoints;
	std::vector<size_t> endPoints;

	bool animationTimer();
	bool vehicleStarterTimer();
	void deleteFinishedVehicles();
	void autoAdder();
};