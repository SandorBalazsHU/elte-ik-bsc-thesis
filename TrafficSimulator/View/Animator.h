/**
 * @name Traffic Simulation
 * @file Animator.h
 * @class Animator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief The animation controller class.
 * Contact: sandorbalazs9402@gmail.com
*/

//Spirit In The Sky

#pragma once

//Empty classes for avoiding the circular includes.
class Render;
class Vehicle;

#include<vector>
#include<set>
#include<SDL.h>
#include <set>
#include "../Model/Graph.h"

/**
 * @brief The animation controller class.
*/
class Animator {
public:

	/**
	 * @brief Animator constructor. Empty.
	 * @param  Empty.
	*/
	Animator(void);

	/**
	 * @brief Animator destructor. Empty.
	 * @param  Empty.
	*/
	~Animator(void);

	/**
	 * @brief Prepare the Animator class.
	 * @param render The current render object.
	*/
	void bind(Render* render);

	/**
	 * @brief The animation update. Callable by every frame for the animation update.
	*/
	void update();

	/**
	 * @brief Prepare the graph representation and all variable for the animation and the vehicle path finding.
	*/
	void finalize();

	/**
	 * @brief Start the animation running.
	*/
	void start();

	/**
	 * @brief Pause the animation.
	*/
	void pause();

	/**
	 * @brief Stop the animation running.
	*/
	void stop();

	/**
	 * @brief Vehicle adder method. Create the 3D and the abstract vehicle too.
	 * @param startPoint The startable vehicle start point.
	 * @param singleVehicle The startable vehicle destination point.
	*/
	void addVehicle(size_t startPoint, bool singleVehicle);

	/**
	 * @brief Getter for an abstract vehicle
	 * @param modelID The needed abstract vehicle.
	 * @return The abstract vehicle.
	*/
	Vehicle* getVehicleModel(size_t modelID);

	/**
	 * @brief Getter for the generated abstract vehicles number.
	 * @return The generated abstract vehicles number.
	*/
	size_t getVehicleModelSize();

	/**
	 * @brief Getter for the generated graph. The graph generated in the finalize method.
	 * @return The generated graph
	*/
	Graph* getGraph();

	/**
	 * @brief Delete all abstract vehicle.
	*/
	void clear();

	/**
	 * @brief Static animator update frequency.
	*/
	static int updateFrequency;

	/**
	 * @brief Static animator starter update frequency.
	*/
	static int vehicleStarterUpdateFrequency;

private:

	/**
	 * @brief Pointer for the current active render.
	*/
	Render* render = NULL;

	/**
	 * @brief Pointer for the current active generated graph.
	*/
	Graph* graph = NULL;

	/**
	 * @brief The current time for animation timing.
	*/
	Uint32 currentTime = 0;

	/**
	 * @brief The previous time for animation timing.
	*/
	Uint32 prevouseTime = 0;

	/**
	 * @brief The current time for vehicle start timing.
	*/
	Uint32 vehicleStarterCurrentTime = 0;

	/**
	 * @brief The previous time for vehicle start timing.
	*/
	Uint32 vehicleStarterPrevouseTime = 0;

	/**
	 * @brief Running animation flag.
	*/
	bool isAnimationRunning = false;

	/**
	 * @brief Soft running flag for one vehicle animation.
	*/
	bool softRunning = false;

	/**
	 * @brief The container for the vehicle models.
	*/
	std::vector<Vehicle> vehicles;

	/**
	 * @brief The current startpoints from the graph.
	*/
	std::vector<size_t> startPoints;

	/**
	 * @brief The current endpoint from the graph.
	*/
	std::vector<size_t> endPoints;

	/**
	 * @brief List of the current stopped vehicles.
	*/
	std::set<size_t> stuckVehicles;

	/**
	 * @brief The started vehicle counter for the secvential starting.
	*/
	size_t startedVehiclesIndex = 0;

	/**
	 * @brief The animation timer calculator.
	 * @return The clock tick for the animation timing.
	*/
	bool animationTimer();

	/**
	 * @brief The vehicle adding timer calculator.
	 * @return The clock tick for the vehicle start timing.
	*/
	bool vehicleStarterTimer();

	/**
	 * @brief Delete the finished vehicles.
	*/
	void deleteFinishedVehicles();

	/**
	 * @brief Timed automate timed secvencial vehicle adder.
	*/
	void autoAdder();

	/**
	 * @brief Random generator.
	 * @param min The needed minimum random number.
	 * @param max The needed maximum random number.
	 * @return The generated random number.
	*/
	int getRandomNumber(int min, int max);

	/**
	 * @brief Reset the statistic counters.
	*/
	void resetStats();

	/**
	 * @brief Update the stuck vehicles list.;
	*/
	void updateStuckVehicleList();

	/**
	 * @brief Check the start road to collisions.
	 * @return Return true if the start road is not free.
	*/
	bool startCollisionDetection();
};