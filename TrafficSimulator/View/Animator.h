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
	void addVehicle(size_t startPoint, bool singleVehicle);
	Vehicle* getVehicleModel(size_t modelID);
	size_t getVehicleModelSize();
	Graph* getGraph();
	void clear();
	static int updateFrequency;
	static int vehicleStarterUpdateFrequency;
private:
	Render* render = NULL;
	Graph* graph = NULL;
	Uint32 currentTime = 0;
	Uint32 prevouseTime = 0;
	Uint32 vehicleStarterCurrentTime = 0;
	Uint32 vehicleStarterPrevouseTime = 0;
	bool isAnimationRunning = false;
	bool softRunning = false;
	//int vehicleStarterUpdateDelay = 100;
	//int startableCarsNumber = 40;
	std::vector<Vehicle> vehicles;
	std::vector<size_t> startPoints;
	std::vector<size_t> endPoints;
	/**
	 * @brief Delay
	*/
	size_t startedVehiclesIndex = 0;

	bool animationTimer();
	bool vehicleStarterTimer();
	void deleteFinishedVehicles();
	void autoAdder();
	int getRandomNumber(int min, int max);
};