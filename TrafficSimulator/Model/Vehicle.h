#pragma once

#include "Graph.h"
#include "Dijkstra.h"
#include "../View/Render.h"
#include "../View/Animator.h"
#include <vector>
#include <SDL.h>

class Vehicle {
public:
	Vehicle(Graph* graph, Render* render, size_t startID, size_t destinationID, size_t object3DiD, size_t ID);
	size_t getID();
	size_t getObject3DiD();
	void update();
	bool isFinished();
	void erase();
	bool isDeleted();
	bool isBlocked();
	void unblock();
	static bool repath;
	static bool collisionPrevention;
	static int vehicleWeight;
	static int collisionCheckDistance;

	size_t destinationID = 0;
	size_t startID = 0;
	Uint32 startTime = 0;
	Uint32 endTime = 0;
	size_t hopCounter = 0;
	int allCost = 0;
private:
	size_t object3DiD = 0;
	size_t ID = 0;
	size_t currentEdgeOnThePath = 0;
	size_t currentRoad = 0;
	size_t currentPointOnTheRoad = 0;
	char track = '1';
	char direction = 'a';
	const size_t standardRoadLenght = 100;
	bool finished = false;
	bool deleted = false;

	bool nextRoadIsBlocked = false;

	Graph* graph;
	Render* render;
	Dijkstra* dijkstra;
	Animator* animator;
	std::vector<size_t> path;

	void nextStep();
	char directionCheck();
	void checkFinish();
	void switchToNextRoad();
	void firstDirectionCheck();
	bool collisionTest(int checkDistance, size_t localCurrentRoad, size_t localCurrentPointOnTheRoad, char direction, char track);
};