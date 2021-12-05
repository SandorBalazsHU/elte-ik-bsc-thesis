#pragma once

#include "Graph.h"
#include "Dijkstra.h"
#include "../View/Render.h"
#include <vector>

class Vehicle {
public:
	Vehicle(Graph* graph, Render* render, size_t startID, size_t destinationID, size_t object3DiD, size_t ID);
	size_t getID();
	size_t getObject3DiD();
	void update();
private:
	size_t object3DiD = 0;
	size_t ID = 0;
	size_t destinationID = 0;
	size_t startID = 0;
	size_t currentEdgeOnThePath = 0;
	size_t currentRoad = 0;
	size_t currentPointOnTheRoad = 0;
	char track = '1';
	char direction = 'a';
	const size_t standardRoadLenght = 100;

	Graph* graph;
	Render* render;
	Dijkstra* dijkstra;
	std::vector<size_t> path;

	void nextStep();
};