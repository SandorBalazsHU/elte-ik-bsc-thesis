/**
 * @name Traffic Simulation
 * @file Point.h
 * @class Point
 * @author S�ndor Bal�zs - AZA6NL
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

private:

	/**
	 * @brief The point id. This is an unique ID.
	*/
	size_t id;

	/**
	 * @brief This point belongs to this edge.
	*/
	size_t edge;

	/**
	 * @brief This point belongs to this side of the edge.
	*/
	char side;

	/**
	 * @brief My graph object.
	*/
	Graph* myGraph;

	/**
	 * @brief Set of connected edges. (Store edgeID-s) (Contain own edge too)
	*/
	std::set<size_t> connectedEdges;
};