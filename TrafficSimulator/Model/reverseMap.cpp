/**
 * @name Traffic Simulation
 * @file reverseMap.cpp
 * @class TrafficSimulator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The reverse map implementation for the Dijkstra algorithm.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "reverseMap.h"

/**
 * @brief Getter for a value by node.
 * @param node The node.
 * @return The value.
*/
int reverseMap::getValueByNode(size_t node) {
    return nodeIndexedStore[node];
}

/**
 * @brief Set a value by node.
 * @param node The node.
 * @param newValue The new value.
*/
void reverseMap::setValueByNode(size_t node, int newValue) {
    int oldValue = nodeIndexedStore[node];
    nodeIndexedStore[node] = newValue;
    changeElementIndex(oldValue, newValue, node);
}

/**
 * @brief Change the index of an element.
 * @param oldIndex The old index.
 * @param newIndex The new index.
 * @param element The element.
*/
void reverseMap::changeElementIndex(const size_t oldIndex, const size_t newIndex, const int element) {
    std::vector<int> findedElements;
    auto it = valueIndexedStore.find(oldIndex);
    while (it != valueIndexedStore.end() && it->first == oldIndex) {
        findedElements.push_back(it->second);
        it++;
    }
    valueIndexedStore.erase(oldIndex);
    for (size_t i = 0; i < findedElements.size(); i++) {
        if (findedElements[i] != element) {
            valueIndexedStore.insert(std::pair<int, int>(oldIndex, findedElements[i]));
        } else {
            valueIndexedStore.insert(std::pair<int, int>(newIndex, findedElements[i]));
        }
    }
}

/**
 * @brief Erase safely an element.
 * @param node The node.
 * @param value The value.
*/
void reverseMap::safeErase(size_t node, int value) {
    std::vector<int> findedElements;
    auto it = valueIndexedStore.find(value);
    while (it != valueIndexedStore.end() && it->first == value) {
        findedElements.push_back(it->second);
        it++;
    }
    valueIndexedStore.erase(value);
    for (size_t i = 0; i < findedElements.size(); i++) {
        if (findedElements[i] != node) valueIndexedStore.insert(std::pair<int, int>(value, findedElements[i]));
    }
}

/**
 * @brief Remove the minimum value element and return it
 * @return std::pair<node,value>
*/
std::pair<size_t, int> reverseMap::removeMinimumValueElement() {
    const size_t minimumNode = valueIndexedStore.begin()->second;
    const int minimumValue = valueIndexedStore.begin()->first;
    safeErase(minimumNode, minimumValue);
    nodeIndexedStore.erase(minimumNode);
    return std::pair<size_t, int>(minimumNode, minimumValue);
}

/**
 * @brief Insert an instance to the reverse map.
 * @param node The node.
 * @param value The value.
*/
void reverseMap::insert(size_t node, int value) {
    nodeIndexedStore.insert(std::pair<size_t, int>(node, value));
    valueIndexedStore.insert(std::pair<int, size_t>(value, node));
}

/**
 * @brief The map is empty?
 * @return The map is empty?
*/
bool reverseMap::empty() {
    return nodeIndexedStore.empty() && valueIndexedStore.empty();
}

/**
 * @brief The node is in the map?
 * @param node The node.
 * @return Is in the map?
*/
bool reverseMap::isIn(size_t node) {
    return nodeIndexedStore.find(node) != nodeIndexedStore.end();
}