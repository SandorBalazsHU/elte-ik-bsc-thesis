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
		for (size_t edge : point->getEdges()){
			this->connectedEdges.insert(edge);
		}
		for (size_t edge : point->getEdges()) {
			myGraph->getEdge(edge)->changePoint(point->getID(), id);
		}
		size_t pointID = point->getID();
		myGraph->deletePoint(pointID);
	}

	/**
	 * @brief Change the point iD and rebind the roads too.
	 * @param id the new id.
	*/
	void Point::reID(size_t id) {
		for (size_t edge : connectedEdges) {
			myGraph->getEdge(edge)->changePoint(this->id, id);
		}
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