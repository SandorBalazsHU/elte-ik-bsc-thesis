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
#include <vector>
#include <set>

/**
 * @brief The abstract Edge implementation.
*/
class Graph {
public:
	Graph(Render* render);
	~Graph();
	Edge* getEdge(size_t edge);
	void deletePoint(size_t point);
	void generateMatrix();
	std::vector<size_t> getPath();
private:
	Render* render;
	std::vector<Edge*> edges;
	std::vector<Point*> points;
	std::vector<size_t> path;
	void initialise();
	void join();
	void rebind();
	void generate();
};