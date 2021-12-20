/**
 * @name Traffic Simulation
 * @file Dijkstra.cpp
 * @class Dijkstra
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Dijkstra algorithm implementation.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "Dijkstra.h"

/**
 * @brief Dijkstra constructor.
 * @param nodesCount The graph node count.
 * @param startNode The graph start node.
*/
Dijkstra::Dijkstra(size_t nodesCount, size_t startNode) : nodesCount(nodesCount), startNode(startNode) {
    init();
}

/**
 * @brief Dijkstra destructor.
*/
Dijkstra::~Dijkstra() {}

/**
 * @brief Adding a new edge to the matrix.
 * @param fromNode The new edge start node.
 * @param toNode The new edge end node.
 * @param weight The new edge weight.
*/
void Dijkstra::addEdge(size_t fromNode, size_t toNode, int weight) {
    matrix[fromNode][toNode] = weight;
}

/**
 * @brief Adding a new double edge to the matrix.
 * @param fromNode The new edge start node.
 * @param toNode The new edge end node.
 * @param weight The new edge weight.
*/
void Dijkstra::addDoubleEdge(size_t node1, size_t node2, int weight) {
    matrix[node1][node2] = weight;
    matrix[node2][node1] = weight;
}

/**
 * @brief Initialize all node and data structures.
*/
void Dijkstra::init() {
    for (size_t i = 0; i < nodesCount; i++) {
        distance.push_back(infinite);
        from.push_back(empty);
        std::vector<int> row;
        for (size_t j = 0; j < nodesCount; j++) {
            row.push_back(empty);
        }
        matrix.push_back(row);
    }
    ready.insert(startNode);
    distance[startNode] = 0;
    initNext(startNode);
    initFrom(startNode);
    from[startNode] = startNode;
}

/**
 * @brief Preparation.
*/
void Dijkstra::prep() {
    ready.insert(startNode);
    distance[startNode] = 0;
    initNext(startNode);
    initFrom(startNode);
    from[startNode] = startNode;
}

/**
 * @brief Initialize the next node.
 * @param startNode The Initializeable node.
*/
void Dijkstra::initNext(size_t startNode) {
    for (size_t node = 0; node < nodesCount; node++) {
        if (matrix[startNode][node] != empty) {
            next.insert(node, matrix[startNode][node]);
        }
    }
}

/**
 * @brief Initialize from.
 * @param startNode Initializeable node.
*/
void Dijkstra::initFrom(size_t startNode) {
    for (size_t i = 0; i < nodesCount; i++) {
        if (matrix[startNode][i] != empty) from[i] = startNode;
    }
}

/**
 * @brief Getter for the ready state.
 * @param node The node.
 * @return Is the node ready?
*/
bool Dijkstra::isReady(size_t node) {
    return ready.find(node) != ready.end();
}

/**
 * @brief Run the algorithm and generate the result vector.
*/
void Dijkstra::run() {
    prep();
    while (!next.empty()) {
        const std::pair<size_t, int> minNext = next.removeMinimumValueElement();
        const size_t minNextNode = minNext.first;
        const int minNextValue = minNext.second;
        ready.insert(minNextNode);
        distance[minNextNode] = minNextValue;
        for (size_t i = 0; i < nodesCount; i++) {
            if (matrix[minNextNode][i] != empty && !isReady(i)) {
                const int newDistance = distance[minNextNode] + matrix[minNextNode][i];
                if (!next.isIn(i)) {
                    next.insert(i, newDistance);
                    from[i] = minNextNode;
                } else {
                    if (next.getValueByNode(i) > newDistance) {
                        next.setValueByNode(i, newDistance);
                        from[i] = minNextNode;
                    }
                }
            }
        }
    }
}

/**
  * @brief Getter for the start node.
  * @return The start node id.
 */
size_t  Dijkstra::getStartNode() {
    return startNode;
}