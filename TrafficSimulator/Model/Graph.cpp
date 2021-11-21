/**
 * @name Traffic Simulation
 * @file Graph.cpp
 * @class Graph
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph class.
 * Contact: sandorbalazs9402@gmail.com
*/
#pragma once

#include "Graph.h"
#include "Edge.h"
#include "Point.h"
#include "../View/Render.h"
#include <vector>
#include <set>

Graph::Graph(Render* render) {
	this->render = render;
}

Graph::~Graph() {
}

Edge* Graph::getEdge(size_t edge) {
	return edges[edge];
}

void Graph::deletePoint(size_t point) {
	delete points[point];
	points[point] = NULL;
}

void Graph::generateMatrix() {
	initialise();
	join();
	rebind();
	generate();
}

void Graph::initialise() {
	for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
		if (render->getDynamicObject(i) != NULL) {
			Point* newPointA = new Point(i, i, this, 'A');
			this->points.push_back(newPointA);
			Point* newPointB = new Point(i + 1, i, this, 'B');
			this->points.push_back(newPointB);
			Edge* newEdge = new Edge(i, i, i, i + 1);
			edges.push_back(newEdge);
		}
	}
}

void Graph::join() {
	for (size_t i = 0; i < edges.size(); i++) {
		if (render->getDynamicObject(i) != NULL) {
			Edge* currentEdge = edges[i];
			Object3Droad* currentRoad = render->getDynamicObject(currentEdge->getRoad3DiD());
			if (currentRoad->stickMarkA != 'Q') {
				if (currentRoad->stickMarkA == 'A') points[currentEdge->getEndpointA()]->join(points[edges[currentRoad->stickA]->getEndpointA()]);
				if (currentRoad->stickMarkA == 'B') points[currentEdge->getEndpointA()]->join(points[edges[currentRoad->stickA]->getEndpointB()]);
			}
			if (currentRoad->stickMarkB != 'Q') {
				if (currentRoad->stickMarkB == 'A') points[currentEdge->getEndpointB()]->join(points[edges[currentRoad->stickB]->getEndpointA()]);
				if (currentRoad->stickMarkB == 'B') points[currentEdge->getEndpointB()]->join(points[edges[currentRoad->stickB]->getEndpointB()]);
			}
		}
	}
}

void Graph::rebind() {

}

void Graph::generate() {

}