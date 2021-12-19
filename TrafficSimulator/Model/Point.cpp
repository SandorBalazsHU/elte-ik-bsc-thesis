/**
 * @name Traffic Simulation
 * @file Point.cpp
 * @class Point
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph Point class.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "Point.h"
#include "Graph.h"
#include <vector>
#include <set>

/**
* @brief The Point constructor.
* @param id The unique ID of the point.
* @param edge This point belongs to this abstract edge.
* @param side This point belongs to this side of the road.
*/
Point::Point(size_t id, size_t edge, Graph* myGraph, char side) {
	this->id = id;
	this->myGraph = myGraph;
	this->edge = edge;
	connectedEdges.insert(edge);
	this->side = side;
}

/*
* @brief Point destructor. (Important for delete)
*/
Point::~Point(void) {
	connectedEdges.clear();
}

/**
* @brief Join this point to an other.
* @param The joinable point.
*/
void Point::join(Point* point) {
	if (this->id != point->getID()) {
		std::set<size_t> pointEdges = point->getEdges();
		for (size_t edge : pointEdges) {
			this->connectedEdges.insert(edge);
		}
		for (size_t edge : pointEdges) {
			myGraph->getEdge(edge)->changePoint(point->getID(), this->id);
		}
		if (point->isStartPoint()) this->setAsStartPoint();
		if (point->isEndPoint()) this->setAsEndpoint();
		size_t pointID = point->getID();
		myGraph->deletePoint(pointID);
	}
}

/**
* @brief Change the point iD and rebind the roads too.
* @param id the new id.
*/
void Point::reID(size_t id) {
	for (size_t edge : connectedEdges) {
		myGraph->getEdge(edge)->changePoint(this->id, id);
	}
	this->id = id;
}

/**
* @brief Getter for the point id. This is an unique ID.
*/
size_t Point::getID() {
	return this->id;
}

/**
* @brief Getter for the point belongs to this edge.
*/
size_t Point::getEdge() {
	return this->edge;
}

/**
* @brief Getter for the side of the edge.
*/
char Point::getSide() {
	return this->side;
}

/**
* @brief geter for the connected edges.
*/
std::set<size_t> Point::getEdges() {
	return this->connectedEdges;
}

/**
* @brief Logical deleting
*/
void Point::erase() {
	this->erased = true;
}

/**
* @brief Is deleted?
* @return Return true if it is deleted.
*/
bool Point::isErased() {
	return this->erased;
}

/**
 * @brief Set the point as a start point.
*/
void Point::setAsStartPoint() {
	this->startPoint = true;
}

/**
 * @brief Set the point as an endpoint.
*/
void Point::setAsEndpoint() {
	this->endPoint = true;
}

/**
 * @brief Is a start point?
 * @return True is this is a start point.
*/
bool Point::isStartPoint() {
	return this->startPoint;
}

/**
 * @brief Is an endpoint?
 * @return True id this is an endpoint.
*/
bool Point::isEndPoint() {
	return this->endPoint;
}

/**
 * @brief Getter for the renderID. If it not have it is -1.
 * @return The renderID. If it not have it is -1.
*/
size_t Point::getRenderID() {
	return this->renderID;
}

/**
 * @brief Setter for the renderID. If it not have it is -1. This is the default.
*/
void Point::setRenderID(size_t id) {
	this->renderID = id;
}

/**
 * @brief Return active vehicles.
 * @return Return active vehicles.
*/
std::vector<size_t> Point::activeVehicles() {
	std::vector<size_t> returnActiveVehicles;
	for (size_t i = 0; i < this->startConfiguration.size(); i++) {
		if (this->startConfiguration[i] != -1) returnActiveVehicles.push_back(this->startConfiguration[i]);
	}
	return returnActiveVehicles;
}

/**
 * @brief Return active endpoints.
 * @return Return active endpoints.
*/
std::vector<size_t> Point::activeEndpoints() {
	std::vector<size_t> returnActiveEndpoints;
	for (size_t i = 0; i < this->endpointsList.size(); i++) {
		if (this->endpointsList[i]) returnActiveEndpoints.push_back(i);
	}
	return returnActiveEndpoints;
}

/**
 * @brief Resetting the running statistics.
*/
void Point::resetStats() {
	this->startableVehicles = this->baseStartableVehicleCount;
	receivedVehicles = 0;
}