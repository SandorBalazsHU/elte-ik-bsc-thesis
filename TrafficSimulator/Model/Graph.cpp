/**
 * @name Traffic Simulation
 * @file Graph.cpp
 * @class Graph
 * @author S�ndor Bal�zs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph class.
 * Contact: sandorbalazs9402@gmail.com
*/
#pragma once

#include "Graph.h"
#include "Edge.h"
#include "Point.h"
#include "Dijkstra.h"
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
	rebind();
	generate();
}

//TODO A t�rl�s korrupt�lhatja az ID-ket.
//�gy gondolom, hogy a t�r�lt �leket is felveszem, de nem olvasom a null v�delem miatt.

void Graph::initialise() {
	size_t j = 0;
	for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
		Edge* newEdge;
		if (render->getDynamicObject(i) != NULL) {
			Point* newPointA = new Point(j, i, this, 'A');
			j++;
			this->points.push_back(newPointA);
			Point* newPointB = new Point(j, i, this, 'B');
			j++;
			this->points.push_back(newPointB);
			newEdge = new Edge(i, i, newPointA->getID(), newPointB->getID(), render->getDynamicObject(i)->getRoadLength());
		} else {
			newEdge = new Edge(-1, -1, -1, -1, -1);
		}
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
		if (!points[i]->isErased()) {
			points[i]->reID(j);
			j++;
		}
	}
}

//Hogy lehetnek ism�tl�d� point ID-k?
void Graph::generate() {
	std::cout << std::endl;
	for (size_t i = 0; i < edges.size(); i++) {
		std::cout << "Edge ID: " << edges[i]->getID() << " Coast: " << edges[i]->getCoast() << " Endpoint A: " << edges[i]->getEndpointA() << " Endpoint B: " << edges[i]->getEndpointB() << std::endl;
	}
	std::cout << std::endl;
	size_t pointCount = 0;
	for (size_t i = 0; i < points.size(); i++) {
		if (!points[i]->isErased()) {
			std::cout << "Point ID: " << points[i]->getID() << " Edges count: " << points[i]->getEdges().size() << " Connected edges: ";
			for (size_t edge : points[i]->getEdges()) {
				std::cout << " - " << edge;
			}
			std::cout << std::endl;
			pointCount++;
		}
	}
	std::cout << std::endl;

	//TODO: 4-1 PARA/////////////////////////////////////////////

	//---------------------------------------------------------------
	Dijkstra dijkstra(pointCount, 0);
	for (size_t i = 0; i < edges.size(); i++) {
		if(edges[i]->getID() != -1) dijkstra.addDoubleEdge(edges[i]->getEndpointA(), edges[i]->getEndpointB(), edges[i]->getCoast());
	}

	dijkstra.run();
	for (size_t i = 0; i < dijkstra.from.size(); i++) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < dijkstra.from.size(); i++) {
		std::cout << dijkstra.from[i] << " ";
	}
	std::cout << std::endl;
	//---------------------------------------------------

	size_t index = 3;
	size_t value = 3;
	do {
		std::cout << "index: " << index << " value: " << dijkstra.from[index] << std::endl;
		for (size_t j = 0; j < edges.size(); j++) {
			if (edges[j]->match(index, dijkstra.from[index])) path.push_back(edges[j]->getRoad3DiD());
		}
	} while ((index = dijkstra.from[index]) != 0);

	//P�rhuzamos �l gondok.????????????????????????????????????????????????????????
	/*for (size_t i = 0; i < pointCount; i++) {
		for (size_t j = 0; j < edges.size(); j++) {
			if (edges[j]->match(i, dijkstra.from[i])) path.push_back(edges[j]->getRoad3DiD());
		}
	}*/
}

std::vector<size_t> Graph::getPath() {
	return path;
}