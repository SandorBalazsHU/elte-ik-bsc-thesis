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
	for (size_t i = 0; i < points.size(); i++)	{
		delete points[i];
	}
	for (size_t i = 0; i < edges.size(); i++)	{
		delete edges[i];
	}
}

Edge* Graph::getEdge(size_t edge) {
	return edges[edge];
}

void Graph::deletePoint(size_t point) {
	points[point]->erase();
}

void Graph::generateMatrix() {
	initialise();
	join();
	//rebind();
	generate();
}

//TODO A törlés korruptálhatja az ID-ket.
//Úgy gondolom, hogy a törölt éleket is felveszem, de nem olvasom a null védelem miatt.

void Graph::initialise() {
	size_t j = 0;
	for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
		Point* newPointA = new Point(j, i, this, 'A');
		j++;
		this->points.push_back(newPointA);
		Point* newPointB = new Point(j, i, this, 'B');
		j++;
		this->points.push_back(newPointB);
		Edge* newEdge = new Edge(i, i, newPointA->getID(), newPointB->getID());
		edges.push_back(newEdge);
	}
}

void Graph::join() {
	for (size_t i = 0; i < edges.size(); i++) {
		if (render->getDynamicObject(i) != NULL) {
			Edge* currentEdge = edges[i];
			Object3Droad* currentRoad = render->getDynamicObject(i);
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
	size_t j = 0;
	for (size_t i = 0; i < points.size(); i++) {
		if (points[i] != NULL) {
			points[i]->reID(j);
			j++;
		}
	}
}

//Hogy lehetnek ismétlõdõ point ID-k?
void Graph::generate() {
	std::cout << std::endl;
	for (size_t i = 0; i < edges.size(); i++) {
		std::cout << "Edge ID: " << edges[i]->getID() << " Endpoint A: " << edges[i]->getEndpointA() << " Endpoint B: " << edges[i]->getEndpointB() << std::endl;
	}
	std::cout << std::endl;
	for (size_t i = 0; i < points.size(); i++) {
		if (!points[i]->isErased()) {
			std::cout << "Point ID: " << points[i]->getID() << " Edges count: " << points[i]->getEdges().size() << " Connected edges: ";
			for (size_t edge : points[i]->getEdges()) {
				std::cout << " - " << edge;
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}