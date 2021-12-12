/**
 * @name Traffic Simulation
 * @file Point.h
 * @class Point
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph Point class.
 * Contact: sandorbalazs9402@gmail.com
*/
#pragma once

class Graph;

#include <vector>
#include <set>

/**
 * @brief The abstract point implementation.
*/
class Point {
public:
 
	/**
	 * @brief The Point constructor.
	 * @param id The unique ID of the point.
	 * @param myGraph the graph witch contains the element.
	 * @param edge This point belongs to this abstract edge. 
	 * @param side This point belongs to this side of the road.
	*/
	Point(size_t id, size_t edge, Graph* myGraph, char side);

	/*
	* @brief Point destructor. (Important for delete)
	*/
	~Point(void);

	/**
	 * @brief Join this point to an other.
	 * @param The joinable point.
	*/
	void join(Point* point);

	/**
	 * @brief Change the point iD and rebind the roads too.
	 * @param id the new id.
	*/
	void reID(size_t id);

	/**
	 * @brief Getter for the point id. This is an unique ID.
	*/
	size_t getID();

	/**
	 * @brief Getter for the point belongs to this edge.
	*/
	size_t getEdge();

	/**
	 * @brief Getter for the side of the edge.
	*/
	char getSide();

	/**
	 * @brief geter for the connected edges.
	*/
	std::set<size_t> getEdges();
	
	/**
	 * @brief Logical deleting
	*/
	void erase();

	/**
	 * @brief Is deleted?
	 * @return Return true if it is deleted.
	*/
	bool isErased();

	/**
	 * @brief Set the point as a start point.
	*/
	void setAsStartPoint();

	/**
	 * @brief Set the point as an endpoint.
	*/
	void setAsEndpoint();

	/**
	 * @brief Is a start point?
	 * @return True is this is a start point.
	*/
	bool isStartPoint();

	/**
	 * @brief Is an endpoint?
	 * @return True id this is an endpoint.
	*/
	bool isEndPoint();

	/**
	 * @brief Getter for the renderID. If it not have it is -1.
	 * @return The renderID. If it not have it is -1.
	*/
	size_t getRenderID();

	/**
	 * @brief Setter for the renderID. If it not have it is -1. This is the default.
	*/
	void setRenderID(size_t id);

	/**
	 * @brief Resetting the running statistics.
	*/
	void resetStats();

	/**
	 * @brief Startable cars if this is a startpoint.
	*/
	std::vector<size_t> startConfiguration;

	/**
	 * @brief Target endpoints if this is a startpoint.
	*/
	std::vector<bool> endpointsList;

	/**
	 * @brief Base startable vehicle count.
	*/
	const int baseStartableVehicleCount = 50;

	/**
	 * @brief The startable cars if this is a startpoint.
	*/
	int startableVehicles = baseStartableVehicleCount;

	/**
	 * @brief The received cars if this is a endpoint.
	*/
	int receivedVehicles = 0;

	/**
	 * @brief Return active vehicles.
	 * @return Return active vehicles.
	*/
	std::vector<size_t> activeVehicles();

	/**
	 * @brief Return active endpoints.
	 * @return Return active endpoints.
	*/
	std::vector<size_t> activeEndpoints();

private:

	/**
	 * @brief The point id. This is an unique ID.
	*/
	size_t id = -1;

	/**
	 * @brief This point belongs to this edge.
	*/
	size_t edge = -1;

	/**
	 * @brief The renderable object ID if it exist. If not exist -1.
	*/
	size_t renderID = -1;

	/**
	 * @brief This point belongs to this side of the edge.
	*/
	char side = 'Q';

	/**
	 * @brief Marker for logic delete
	*/
	bool erased = false;

	/**
	 * @brief Mark for start points.
	*/
	bool startPoint = false;

	/**
	 * @brief mark for endpoints.
	*/
	bool endPoint = false;

	/**
	 * @brief My graph object.
	*/
	Graph* myGraph;

	/**
	 * @brief Set of connected edges. (Store edgeID-s) (Contain own edge too)
	*/
	std::set<size_t> connectedEdges;
};