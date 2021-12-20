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

/**
 * @brief Edge constructor.
 * @param id The vector id.
 * @param road3DiD The render ID.
 * @param endpointA The endpoint A ID.
 * @param endpointB The endpoint B ID.
 * @param coast The road cost.
*/
Edge::Edge(size_t id, size_t road3DiD, size_t endpointA, size_t endpointB, int coast) {
	this->id = id;
	this->road3DiD = road3DiD;
	this->endpointA = endpointA;
	this->endpointB = endpointB;
	this->coast = coast;
}

/**
 * @brief Edge destructor.
*/
Edge::~Edge() {

}

/**
 * @brief Change point.
 * @param originalPoint Original point.
 * @param newPoint New point.
*/
void Edge::changePoint(size_t originalPoint, size_t newPoint) {
	if (this->endpointA == originalPoint) this->endpointA = newPoint;
	if (this->endpointB == originalPoint) this->endpointB = newPoint;
}

/**
 * @brief Getter for the vector ID.
 * @return The vector ID.
*/
size_t Edge::getID() {
	return this->id;
}

/**
 * @brief Getter for the render ID.
 * @return The render ID.
*/
size_t Edge::getRoad3DiD() {
	return this->road3DiD;
}

/**
 * @brief Getter for the Endpoint A vector ID.
 * @return The Endpoint A vector ID.
*/
size_t Edge::getEndpointA() {
	return this->endpointA;
}

/**
 * @brief Getter for the Endpoint B vector ID.
 * @return The Endpoint B vector ID.
*/
size_t Edge::getEndpointB() {
	return this->endpointB;
}

/**
 * @brief Getter for the cost.
 * @return The cost.
*/
int Edge::getCoast() {
	return this->coast + this->vehicleCoast;
}

/**
 * @brief Getter for the length.
 * @return The current legth.
*/
int Edge::getLength() {
	return this->coast;
}

/**
 * @brief Getter for the vehicle cost.
 * @return The vehicle cost.
*/
int Edge::getVehicleCoast() {
	return this->vehicleCoast;
}

/**
 * @brief Getter for the vehicle count.
 * @return The vehicle count.
*/
size_t Edge::getVehicleCount() {
	return this->vehicleCount;
}

/**
 * @brief Getter for the all vehicle count.
 * @return The all vehicle count.
*/
size_t Edge::getAllVehicleCount() {
	return this->allVehicleCount;
}

/**
 * @brief Add vehicle to the edge statistic.
 * @param The cost.
 * @param repath Repath is working?
*/
void Edge::addVehicle(int c, bool repath) {
	if(repath) this->vehicleCoast += c;
	this->vehicleCount++;
	this->allVehicleCount++;
}

/**
 * @brief Remove vehicle to the edge statistic.
 * @param The cost.
 * @param repath Repath is working?
*/
void Edge::removeVehicle(int c, bool repath) {
	if(repath) this->vehicleCoast = this->vehicleCoast - c;
	this->vehicleCount--;
}

/**
 * @brief This edges are parallel?
 * @param endpointA Endpoint A.
 * @param endpointB Endpoint B
 * @return True if the two edge is parallel.
*/
bool Edge::match(size_t endpointA, size_t endpointB) {
	return ((endpointA == this->endpointA) && (endpointB == this->endpointB)) || ((endpointA == this->endpointB) && (endpointB == this->endpointA));
}

/**
 * @brief Resetting the edge statistic.
*/
void Edge::resetStats() {
	vehicleCoast = 0;
	vehicleCount = 0;
	allVehicleCount = 0;
}