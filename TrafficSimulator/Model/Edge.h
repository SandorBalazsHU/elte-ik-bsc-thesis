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
	/**
	 * @brief Edge constructor.
	 * @param id The vector id.
	 * @param road3DiD The render ID.
	 * @param endpointA The endpoint A ID.
	 * @param endpointB The endpoint B ID.
	 * @param coast The road cost.
	*/
	Edge(size_t id, size_t road3DiD, size_t endpointA, size_t endpointB, int coast);

	/**
	 * @brief Edge destructor.
	*/
	~Edge();

	/**
	 * @brief Change point.
	 * @param originalPoint Original point. 
	 * @param newPoint New point.
	*/
	void changePoint(size_t originalPoint, size_t newPoint);

	/**
	 * @brief Getter for the vector ID.
	 * @return The vector ID.
	*/
	size_t getID();

	/**
	 * @brief Getter for the render ID.
	 * @return The render ID.
	*/
	size_t getRoad3DiD();

	/**
	 * @brief Getter for the Endpoint A vector ID.
	 * @return The Endpoint A vector ID.
	*/
	size_t getEndpointA();

	/**
	 * @brief Getter for the Endpoint B vector ID.
	 * @return The Endpoint B vector ID.
	*/
	size_t getEndpointB();

	/**
	 * @brief Getter for the cost.
	 * @return The cost.
	*/
	int getCoast();

	/**
	 * @brief Getter for the length.
	 * @return The current legth.
	*/
	int getLength();

	/**
	 * @brief Getter for the vehicle cost.
	 * @return The vehicle cost.
	*/
	int getVehicleCoast();

	/**
	 * @brief Getter for the vehicle count.
	 * @return The vehicle count.
	*/
	size_t getVehicleCount();

	/**
	 * @brief Getter for the all vehicle count.
	 * @return The all vehicle count.
	*/
	size_t getAllVehicleCount();

	/**
	 * @brief Add vehicle to the edge statistic.
	 * @param The cost.
	 * @param repath Repath is working?
	*/
	void addVehicle(int c, bool repath);

	/**
	 * @brief Remove vehicle to the edge statistic.
	 * @param The cost.
	 * @param repath Repath is working?
	*/
	void removeVehicle(int c, bool repath);

	/**
	 * @brief This edges are parallel?
	 * @param endpointA Endpoint A.
	 * @param endpointB Endpoint B
	 * @return True if the two edge is parallel.
	*/
	bool match(size_t endpointA, size_t endpointB);

	/**
	 * @brief Resetting the edge statistic.
	*/
	void resetStats();

private:

	/**
	 * @brief The vector ID.
	*/
	size_t id = -1;

	/**
	 * @brief The render ID.
	*/
	size_t road3DiD = -1;

	/**
	 * @brief The edge endpoint A vector ID.
	*/
	size_t endpointA = -1;

	/**
	 * @brief The edge endpoint B vector ID.
	*/
	size_t endpointB = -1;

	/**
	 * @brief The cost.
	*/
	int coast = 0;

	/**
	 * @brief The current vehicle cost.
	*/
	int vehicleCoast = 0;

	/**
	 * @brief The current vehicle cost.
	*/
	size_t vehicleCount = 0;

	/**
	 * @brief The all vehicle cost.
	*/
	size_t allVehicleCount = 0;
};