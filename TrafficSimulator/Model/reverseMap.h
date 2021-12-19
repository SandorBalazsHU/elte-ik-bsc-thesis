/**
 * @name Traffic Simulation
 * @file reverseMap.h
 * @class TrafficSimulator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The reverse map implementation for the Dijkstra algorithm.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include <vector>
#include <map>
#include <set>

/**
 * @brief The reverse map implementation for the Dijkstra algorithm.
*/
class reverseMap {
public:

    /**
     * @brief Constructor for the reverse map.
    */
    reverseMap() {}

    /**
     * @brief Destructor for the reverse map.
    */
    ~reverseMap() {}

    /**
     * @brief Getter for a value by node.
     * @param node The node.
     * @return The value.
    */
    int getValueByNode(size_t node);

    /**
     * @brief Set a value by node.
     * @param node The node.
     * @param newValue The new value.
    */
    void setValueByNode(size_t node, int newValue);

    /**
     * @brief Remove the minimum value element and return it
     * @return std::pair<node,value>
    */
    std::pair<size_t, int> removeMinimumValueElement();

    /**
     * @brief Insert an instance to the reverse map.
     * @param node The node.
     * @param value The value.
    */
    void insert(size_t node, int value);

    /**
     * @brief The map is empty?
     * @return The map is empty?
    */
    bool empty();

    /**
     * @brief The node is in the map?
     * @param node The node.
     * @return Is in the map?
    */
    bool isIn(size_t node);

    /**
     * @brief THe node index storage.
    */
    std::map<size_t, int> nodeIndexedStore;

    /**
     * @brief The value index storage.
    */
    std::multimap<int, size_t> valueIndexedStore;

private:

    /**
     * @brief Change the index of an element. 
     * @param oldIndex The old index.
     * @param newIndex The new index.
     * @param element The element.
    */
    void changeElementIndex(const size_t oldIndex, const size_t newIndex, const int element);

    /**
     * @brief Erase safely an element.
     * @param node The node.
     * @param value The value.
    */
    void safeErase(size_t node, int value);
};