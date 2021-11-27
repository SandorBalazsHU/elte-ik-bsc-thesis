#pragma once
#include <vector>
#include <map>
#include <set>
#include "reverseMap.h"


class Dijkstra {
public:
    Dijkstra(size_t nodesCount, size_t startNode) : nodesCount(nodesCount), startNode(startNode) {
        init();
    }
    ~Dijkstra() {}
    void addEdge(size_t fromNode, size_t toNode, int weight);
    void addDoubleEdge(size_t node1, size_t node2, int weight);
    size_t getStartNode();
    //std::vector<std::vector<size_t>> getMatrix();
    void run();
    std::vector<size_t> from;
    std::vector<std::vector<int>> matrix;
private:
    const int infinite = INT_MAX - 1;
    const int empty = -1;
    size_t nodesCount;
    size_t startNode;
    //std::vector<std::vector<size_t>> matrix;
    std::set<size_t> ready;
    std::vector<int> distance;
    reverseMap next;
    //std::vector<size_t> from;

    void updateDistance(size_t node);
    void initNext(size_t startNode);
    void initFrom(size_t startNode);
    size_t getNextValue(size_t node);
    void updateFrom(size_t node);
    bool isReady(size_t node);
    void init();
    void prep();
};