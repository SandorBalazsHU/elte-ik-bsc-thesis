/**
 * @name Traffic Simulation
 * @file Edge.h
 * @class Edge
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph Edge class.
 * Contact: sandorbalazs9402@gmail.com
*/
#pragma once

/**
 * @brief The abstract Edge implementation.
*/
class Edge {
public:
	Edge(size_t id, size_t road3DiD, size_t endpointA, size_t endpointB, int coast);
	~Edge();
	void changePoint(size_t originalPoint, size_t newPoint);
	size_t getID();
	size_t getRoad3DiD();
	size_t getEndpointA();
	size_t getEndpointB();
	int getCoast();
	void addVehicleCoast(int c);
	void removeVehicleCoast(int c);
	bool match(size_t endpointA, size_t endpointB);
private:
	size_t id = -1;
	size_t road3DiD = -1;
	size_t endpointA = -1;
	size_t endpointB = -1;
	int coast = 0;
	int vehicleCoast = 0;
};