/**
 * @name Traffic Simulation
 * @file Edge.cpp
 * @class Edge
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph Edge class.
 * Contact: sandorbalazs9402@gmail.com
*/
#include "Edge.h"
#include <vector>
#include <set>


Edge::Edge(size_t id, size_t road3DiD, size_t endpointA, size_t endpointB, int coast) {
	this->id = id;
	this->road3DiD = road3DiD;
	this->endpointA = endpointA;
	this->endpointB = endpointB;
	this->coast = coast;
}

Edge::~Edge() {

}

void Edge::changePoint(size_t originalPoint, size_t newPoint) {
	if (this->endpointA == originalPoint) this->endpointA = newPoint;
	if (this->endpointB == originalPoint) this->endpointB = newPoint;
}

size_t Edge::getID() {
	return this->id;
}

size_t Edge::getRoad3DiD() {
	return this->road3DiD;
}

size_t Edge::getEndpointA() {
	return this->endpointA;
}

size_t Edge::getEndpointB() {
	return this->endpointB;
}

int Edge::getCoast() {
	return this->coast;
}

bool Edge::match(size_t endpointA, size_t endpointB) {
	return ((endpointA == this->endpointA) && (endpointB == this->endpointB)) || ((endpointA == this->endpointB) && (endpointB == this->endpointA));
}