#include "Dijkstra.h"

void Dijkstra::addEdge(int fromNode, int toNode, int weight) {
    matrix[fromNode][toNode] = weight;
}

void Dijkstra::addDoubleEdge(int node1, int node2, int weight) {
    matrix[node1][node2] = weight;
    matrix[node2][node1] = weight;
}

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

void Dijkstra::prep() {
    ready.insert(startNode);
    distance[startNode] = 0;
    initNext(startNode);
    initFrom(startNode);
    from[startNode] = startNode;
}

void Dijkstra::initNext(int startNode) {
    for (size_t node = 0; node < nodesCount; node++) {
        if (matrix[startNode][node] != empty) {
            next.insert(node, matrix[startNode][node]);
        }
    }
}

void Dijkstra::initFrom(int startNode) {
    for (size_t i = 0; i < nodesCount; i++) {
        if (matrix[startNode][i] != empty) from[i] = startNode;
    }
}

bool Dijkstra::isReady(int node) {
    return ready.find(node) != ready.end();
}

void Dijkstra::run() {
    prep();
    while (!next.empty()) {
        const std::pair<int, int> minNext = next.removeMinimumValueElement();
        const int minNextNode = minNext.first;
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

int  Dijkstra::getStartNode() {
    return startNode;
}