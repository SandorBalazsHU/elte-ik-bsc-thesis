/**
 * @name Traffic Simulation
 * @file Dijkstra.h
 * @class Dijkstra
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Dijkstra algorithm implementation.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include <vector>
#include <map>
#include <set>
#include "reverseMap.h"

/**
 * @brief Dijkstra algorithm implementation.
*/
class Dijkstra {
public:
    /**
     * @brief Dijkstra constructor.
     * @param nodesCount The graph node count.
     * @param startNode The graph start node.
    */
    Dijkstra(size_t nodesCount, size_t startNode);

    /**
     * @brief Dijkstra destructor.
    */
    ~Dijkstra();

    /**
     * @brief Adding a new edge to the matrix.
     * @param fromNode The new edge start node.
     * @param toNode The new edge end node.
     * @param weight The new edge weight.
    */
    void addEdge(size_t fromNode, size_t toNode, int weight);

    /**
     * @brief Adding a new double edge to the matrix.
     * @param fromNode The new edge start node.
     * @param toNode The new edge end node.
     * @param weight The new edge weight.
    */
    void addDoubleEdge(size_t node1, size_t node2, int weight);

    /**
     * @brief Getter for the start node.
     * @return The start node id.
    */
    size_t getStartNode();

    /**
     * @brief Run the algorithm and generate the result vector.
    */
    void run();

    /**
     * @brief The algorithm result matrix.
    */
    std::vector<size_t> from;

    /**
     * @brief The graph representation matrix.
    */
    std::vector<std::vector<int>> matrix;

private:

    /**
     * @brief Infinite mark.
    */
    const int infinite = INT_MAX - 1;

    /**
     * @brief Empty mark.
    */
    const int empty = -1;

    /**
     * @brief Node count.
    */
    size_t nodesCount;

    /**
     * @brief Start node.
    */
    size_t startNode;

    /**
     * @brief Ready nodes list.
    */
    std::set<size_t> ready;

    /**
     * @brief Distances list.
    */
    std::vector<int> distance;

    /**
     * @brief The next reverse map.
    */
    reverseMap next;

    /**
     * @brief Update the distance.
     * @param node The updatble node.
    */
    void updateDistance(size_t node);

    /**
     * @brief Initialize the next node.
     * @param startNode The Initializeable node.
    */
    void initNext(size_t startNode);

    /**
     * @brief Initialize from.
     * @param startNode Initializeable node.
    */
    void initFrom(size_t startNode);

    /**
     * @brief Getter for the next value.
     * @param node The node.
     * @return The next value.
    */
    size_t getNextValue(size_t node);

    /**
     * @brief Update from.
     * @param node The updatable node.
    */
    void updateFrom(size_t node);

    /**
     * @brief Getter for the ready state.
     * @param node The node.
     * @return Is the node ready?
    */
    bool isReady(size_t node);

    /**
     * @brief Initialize all node and data structures.
    */
    void init();

    /**
     * @brief Preparation.
    */
    void prep();
};