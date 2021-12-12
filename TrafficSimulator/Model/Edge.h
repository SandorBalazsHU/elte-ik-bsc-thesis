/**
 * @name Traffic Simulation
 * @file Edge.h
 * @class Edge
 * @author S�ndor Bal�zs - AZA6NL
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
	int getLength();
	int getVehicleCoast();
	size_t getVehicleCount();
	size_t getAllVehicleCount();
	void addVehicle(int c, bool repath);
	void removeVehicle(int c, bool repath);
	bool match(size_t endpointA, size_t endpointB);
	void resetStats();
private:
	size_t id = -1;
	size_t road3DiD = -1;
	size_t endpointA = -1;
	size_t endpointB = -1;
	int coast = 0;
	int vehicleCoast = 0;
	size_t vehicleCount = 0;
	size_t allVehicleCount = 0;
};