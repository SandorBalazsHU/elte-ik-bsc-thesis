/**
 * @name Traffic Simulation
 * @file Vehicle.h
 * @class Vehicle
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Vehicle model representation.
 * Contact: sandorbalazs9402@gmail.com
*/

//NEVER FADE AWAY!

#pragma once

#include "Graph.h"
#include "Dijkstra.h"
#include "../View/Render.h"
#include "../View/Animator.h"
#include <vector>
#include <SDL.h>

/**
 * @brief Vehicle model representation.
*/
class Vehicle {
public:

	/**
	 * @brief Constructor for the Vehicle.
	 * @param graph The current graph.
	 * @param render The pointer for the render.
	 * @param startID The startpoint ID.
	 * @param destinationID The destination endpoint.
	 * @param object3DiD The 3D render ID.
	 * @param ID The current ID.
	*/
	Vehicle(Graph* graph, Render* render, size_t startID, size_t destinationID, size_t object3DiD, size_t ID);

	/**
	 * @brief Getter for the Vehicle ID.
	 * @return vehicle ID.
	*/
	size_t getID();

	/**
	 * @brief Getter for the render ID.
	 * @return The render ID.
	*/
	size_t getObject3DiD();

	/**
	 * @brief Update the Vehicle status.
	*/
	void update();

	/**
	 * @brief Getter for the finish flag.
	 * @return 
	*/
	bool isFinished();

	/**
	 * @brief Set the vehicle deleted.
	*/
	void erase();

	/**
	 * @brief Getter for the deleted flag.
	 * @return The deleted flag.
	*/
	bool isDeleted();

	/**
	 * @brief Getter for the blocked flag.
	 * @return The blocked flag.
	*/
	bool isBlocked();

	/**
	 * @brief Unblock the vehicle.
	*/
	void unblock();

	/**
	 * @brief Repath function static flag.
	*/
	static bool repath;

	/**
	 * @brief Collision detection static flag.
	*/
	static bool collisionPrevention;

	/**
	 * @brief The vehicle weight in the graph.
	*/
	static int vehicleWeight;

	/**
	 * @brief The collision detection static distance.
	*/
	static int collisionCheckDistance;

	/**
	 * @brief the destination point id.
	*/
	size_t destinationID = 0;

	/**
	 * @brief The start point ID.
	*/
	size_t startID = 0;

	/**
	 * @brief The start time.
	*/
	Uint32 startTime = 0;

	/**
	 * @brief The end time.
	*/
	Uint32 endTime = 0;

	/**
	 * @brief The hop counter.
	*/
	size_t hopCounter = 0;

	/**
	 * @brief The cost calculator.
	*/
	int allCost = 0;

private:

	/**
	 * @brief The render ID.
	*/
	size_t object3DiD = 0;

	/**
	 * @brief The vehicle moddel ID.
	*/
	size_t ID = 0;

	/**
	 * @brief The current road number on the path.
	*/
	size_t currentEdgeOnThePath = 0;

	/**
	 * @brief The current road ID.
	*/
	size_t currentRoad = 0;

	/**
	 * @brief The current point on the current road.
	*/
	size_t currentPointOnTheRoad = 0;

	/**
	 * @brief The track marker.
	*/
	char track = '1';

	/**
	 * @brief the traveling direction marker.
	*/
	char direction = 'a';

	/**
	 * @brief The standard road length constant.
	*/
	const size_t standardRoadLenght = 100;

	/**
	 * @brief The finished flag.
	*/
	bool finished = false;

	/**
	 * @brief The deleted flag.
	*/
	bool deleted = false;

	/**
	 * @brief The blocked flag.
	*/
	bool nextRoadIsBlocked = false;

	/**
	 * @brief The pointer to the current graph.
	*/
	Graph* graph;

	/**
	 * @brief The pointer to the current render.
	*/
	Render* render;

	/**
	 * @brief The pointer to the vehicle's own Dijkstra object..
	*/
	Dijkstra* dijkstra;

	/**
	 * @brief The pointer to the current animator object.
	*/
	Animator* animator;

	/**
	 * @brief The current calculated path.
	*/
	std::vector<size_t> path;

	/**
	 * @brief Step to the next point.
	*/
	void nextStep();

	/**
	 * @brief Check the vehicle direction and track.
	*/
	char directionCheck();

	/**
	 * @brief Check the path finishing.
	*/
	void checkFinish();

	/**
	 * @brief Check the next road.
	*/
	void switchToNextRoad();

	/**
	 * @brief Calculate the first start direction and track.
	*/
	void firstDirectionCheck();

	/**
	 * @brief Check the road collision.
	*/
	bool collisionTest(int checkDistance, size_t localCurrentRoad, size_t localCurrentPointOnTheRoad, char direction, char track);
};