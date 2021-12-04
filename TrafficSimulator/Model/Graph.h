/**
 * @name Traffic Simulation
 * @file Graph.h
 * @class Graph
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph class.
 * Contact: sandorbalazs9402@gmail.com
*/
#pragma once

class Render;

#include "Edge.h"
#include "Point.h"
#include "Vehicle.h"
#include "Dijkstra.h"
#include <vector>
#include <set>

/**
 * @brief The abstract Edge implementation.
*/
class Graph {
public:
	Graph(Render* render);
	~Graph();
	size_t getEdgesNumber();
	Edge* getEdge(size_t edge);
	Edge* getEdgeByID(size_t edge);
	size_t getPointsNumber();
	Point* getPoint(size_t point);
	Point* getPointByID(size_t point);
	void deletePoint(size_t point);
	void generateGraph();
	Dijkstra* generateDijkstra(size_t startPoint);
	std::vector<size_t> getPath(Dijkstra* dijkstra, size_t target);
	std::vector<size_t> getStartPoints();
	std::vector<size_t> getEndPoints();

	private:
	Render* render;
	std::vector<Edge*> edges;
	std::vector<Point*> points;
	std::vector<Vehicle*> vehicles;

	size_t pointCount = 0;
	void lockRoads();
	void initialise();
	void join();
	void rebind();
};