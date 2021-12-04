/**
 * @name Traffic Simulation
 * @file Graph.cpp
 * @class Graph
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph class.
 * Contact: sandorbalazs9402@gmail.com
 * Spirit In The Sky
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

//TODO Máshol nem érdemes már korábban törölni õket?
Graph::~Graph() {
	for (size_t i = 0; i < points.size(); i++)	{
		delete points[i];
	}
	for (size_t i = 0; i < edges.size(); i++)	{
		delete edges[i];
	}
}

size_t Graph::getEdgesNumber() {
	return this->edges.size();
}

Edge* Graph::getEdge(size_t edge) {
	return this->edges[edge];
}

Edge* Graph::getEdgeByID(size_t edge) {
	for (size_t i = 0; i < this->edges.size(); i++) {
		if (this->edges[i]->getID() == edge) return this->edges[i];
	}
	return NULL;
}

size_t Graph::getPointsNumber() {
	return this->points.size();
}

Point* Graph::getPoint(size_t point) {
	return this->points[point];
}


Point* Graph::getPointByID(size_t point) {
	for (size_t i = 0; i < this->points.size(); i++) {
		if (this->points[i]->getID() == point) return this->points[i];
	}
	return NULL;
}

void Graph::deletePoint(size_t point) {
	points[point]->erase();
}

void Graph::generateGraph() {
	lockRoads();
	initialise();
	join();
	rebind();
}

Dijkstra* Graph::generateDijkstra(size_t startPoint) {
	Dijkstra* dijkstra = new Dijkstra(this->pointCount, startPoint);
	for (size_t i = 0; i < this->edges.size(); i++) {
		if (this->edges[i]->getID() != -1) dijkstra->addDoubleEdge(this->edges[i]->getEndpointA(), this->edges[i]->getEndpointB(), this->edges[i]->getCoast());
	}
	dijkstra->run();
	return dijkstra;
}

std::vector<size_t> Graph::getPath(Dijkstra* dijkstra, size_t target) {
	std::vector<size_t> path;
	size_t index = target;
	size_t value = -1;
	do {
		for (size_t j = 0; j < this->edges.size(); j++) {
			if (this->edges[j]->match(index, dijkstra->from[index])) path.push_back(this->edges[j]->getID());
		}
		if (index == value) break;
	} while ((index = dijkstra->from[index]) != dijkstra->getStartNode());

	std::set<size_t> deletable;
	for (size_t i = 0; i < path.size(); i++) {
		std::vector<size_t> repetitions;
		size_t a = this->edges[path[i]]->getEndpointA();
		size_t b = this->edges[path[i]]->getEndpointB();
		for (size_t j = 0; j < path.size(); j++) if (this->edges[path[j]]->match(a, b)) repetitions.push_back(path[j]);

		if (repetitions.size() > 1) {
			size_t minID = 0;
			int minValue = edges[repetitions[0]]->getCoast();
			for (size_t j = 0; j < repetitions.size(); j++) {
				if (this->edges[repetitions[j]]->getCoast() <= minValue) {
					minValue = this->edges[repetitions[j]]->getCoast();
					minID = j;
				}
			}
			repetitions.erase(repetitions.begin() + minID);
			for (size_t j = 0; j < repetitions.size(); j++) {
				deletable.insert(repetitions[j]);
			}
		}
	}

	std::vector<size_t> realPath;
	for (int i = path.size()-1; i >= 0 ; i--) {
		if(deletable.find(path[i]) == deletable.end()) realPath.push_back(this->edges[path[i]]->getRoad3DiD());
	}
	return realPath;
}

/*std::vector<size_t> Graph::getPath(Dijkstra* dijkstra, size_t target) {
	std::vector<size_t> path;
	size_t index = target;
	size_t value = -1;
	do {
		for (size_t j = 0; j < this->edges.size(); j++) {
			if (this->edges[j]->match(index, dijkstra->from[index])) path.push_back(this->edges[j]->getRoad3DiD());
		}
		if (index == value) break;
	} while ((index = dijkstra->from[index]) != dijkstra->getStartNode());
	return path;
}*/

void Graph::lockRoads() {
	for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
		if (render->getDynamicObject(i) != NULL) {
			render->updateDynamicObject(i);
		}
	}
}

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
			newEdge = new Edge(i, i, newPointA->getID(), newPointB->getID(), (int) render->getDynamicObject(i)->getRoadLength());
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
			if (currentRoad->markerA != -1) {
				if (render->getObject(currentRoad->markerA)->getName() == "Start sign") {
					points[currentEdge->getEndpointA()]->setAsStartPoint();
					points[currentEdge->getEndpointA()]->setRenderID(currentRoad->markerA);
				}
				if (render->getObject(currentRoad->markerA)->getName() == "Stop sign") {
					points[currentEdge->getEndpointA()]->setAsEndpoint();
					points[currentEdge->getEndpointA()]->setRenderID(currentRoad->markerA);
				}
			}
			if (currentRoad->markerB != -1) {
				if (render->getObject(currentRoad->markerB)->getName() == "Start sign") {
					points[currentEdge->getEndpointB()]->setAsStartPoint();
					points[currentEdge->getEndpointB()]->setRenderID(currentRoad->markerB);
				}
				if (render->getObject(currentRoad->markerB)->getName() == "Stop sign") {
					points[currentEdge->getEndpointB()]->setAsEndpoint();
					points[currentEdge->getEndpointB()]->setRenderID(currentRoad->markerB);
				}
			}
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
	this->pointCount = j;

	for (size_t i = 0; i < points.size(); i++) {
		if (!points[i]->isErased() && (points[i]->isEndPoint() || points[i]->isStartPoint())) render->getObject(points[i]->getRenderID())->setModelID(i);
	}
	for (size_t i = 0; i < edges.size(); i++) {
		if (edges[i]->getID() != -1) render->getDynamicObject(edges[i]->getRoad3DiD())->modelID = i;
	}
}

std::vector<size_t> Graph::getStartPoints() {
	std::vector<size_t> startPoints;
	for (size_t i = 0; i < points.size(); i++) {
		if (!points[i]->isErased()) {
			if (points[i]->isStartPoint()) startPoints.push_back(this->points[i]->getID());
		}
	}
	return startPoints;
}

std::vector<size_t> Graph::getEndPoints() {
	std::vector<size_t> endPoints;
	for (size_t i = 0; i < points.size(); i++) {
		if (!points[i]->isErased()) {
			if (points[i]->isEndPoint()) endPoints.push_back(this->points[i]->getID());
		}
	}
	return endPoints;
}