#pragma once
#include <vector>
#include <map>
#include <set>
#include "reverseMap.h"


class Dijkstra {
public:
    Dijkstra(int nodesCount, int startNode) : nodesCount(nodesCount), startNode(startNode) {
        init();
    }
    ~Dijkstra() {}
    void addEdge(int fromNode, int toNode, int weight);
    void addDoubleEdge(int node1, int node2, int weight);
    std::vector<std::vector<int>> getMatrix();
    void run();
    std::vector<int> from;
    std::vector<std::vector<int>> matrix;
private:
    const int infinite = INT_MAX - 1;
    const int empty = -1;
    int nodesCount;
    int startNode;
    //std::vector<std::vector<int>> matrix;
    std::set<int> ready;
    std::vector<int> distance;
    reverseMap next;
    //std::vector<int> from;

    void updateDistance(int node);
    void initNext(int startNode);
    void initFrom(int startNode);
    int getNextValue(int node);
    void updateFrom(int node);
    bool isReady(int node);
    void init();
    void prep();
};