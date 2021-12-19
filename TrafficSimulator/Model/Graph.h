/**
 * @name Traffic Simulation
 * @file Graph.h
 * @class Graph
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.20.
 * @brief The abstract graph class.
 * Contact: sandorbalazs9402@gmail.com
*/
#pragma once

/**
 * @brief Circular include prevention.
*/
class Render;

#include "Edge.h"
#include "Point.h"
#include "Dijkstra.h"
#include <vector>
#include <set>

/**
 * @brief The abstract Edge implementation.
*/
class Graph {
public:

	/**
	 * @brief Graph constructor.
	 * @param render Pointer to the current render.
	*/
	Graph(Render* render);

	/**
	 * @brief Graph destructor.
	*/
	~Graph();

	/**
	 * @brief Getter for the edge count.
	 * @return The edge count.
	*/
	size_t getEdgesNumber();

	/**
	 * @brief Getter for the edges by vector id.
	 * @param edge The needed edge vector ID.
	 * @return The needed edge.
	*/
	Edge* getEdge(size_t edge);

	/**
	 * @brief Getter for the edges by real ID.
	 * @param edge The needed edge real ID.
	 * @return The needed edge.
	*/
	Edge* getEdgeByID(size_t edge);

	/**
	 * @brief Getter for the points count.
	 * @return The points count.
	*/
	size_t getPointsNumber();

	/**
	 * @brief Getter for the points by vector id.
	 * @param edge The needed point vector ID.
	 * @return The needed point.
	*/
	Point* getPoint(size_t point);

	/**
	 * @brief Getter for the points by real id.
	 * @param edge The needed point real ID.
	 * @return The needed point.
	*/
	Point* getPointByID(size_t point);

	/**
	 * @brief Delete a point.
	 * @param point The deletable point.
	*/
	void deletePoint(size_t point);

	/**
	 * @brief Generate the graph by the renderable roads.
	*/
	void generateGraph();

	/**
	 * @brief Generate Adjacency matrix from the graph and the Dijkstra object.
	 * @param startPoint The start point in the graph.
	 * @return The generated Dijkstra object.
	*/
	Dijkstra* generateDijkstra(size_t startPoint);

	/**
	 * @brief Generate path matrix from the Dijkstra object results.
	 * @param Dijkstra The Dijkstra object.
	 * @param target The target point.
	 * @return The path.
	*/
	std::vector<size_t> getPath(Dijkstra* dijkstra, size_t target);

	/**
	 * @brief Getter for the start point list.
	 * @return The startpoint list.
	*/
	std::vector<size_t> getStartPoints();

	/**
	 * @brief Getter for the end point list.
	 * @return The endpoint list.
	*/
	std::vector<size_t> getEndPoints();

	private:

	/**
	 * @brief The render pointer.
	*/
	Render* render;

	/**
	 * @brief The list of the edges in the graph.
	*/
	std::vector<Edge*> edges;

	/**
	 * @brief The list of the points in the graph.
	*/
	std::vector<Point*> points;

	/**
	 * @brief The real point count. (Without deleted points.)
	*/
	size_t pointCount = 0;

	/**
	 * @brief Lock the roads.
	*/
	void lockRoads();

	/**
	 * @brief initialize the graph.
	*/
	void initialize();

	/**
	 * @brief Join the multiple points.
	*/
	void join();

	/**
	 * @brief Rebind the real ID-s.
	*/
	void rebind();
};