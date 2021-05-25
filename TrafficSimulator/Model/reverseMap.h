#pragma once
#include <vector>
#include <map>
#include <set>

class reverseMap {
public:
    reverseMap() {}
    ~reverseMap() {}
    int getValueByNode(int node);
    void setValueByNode(int node, int newValue);
    /**
     * @brief Remove the minimum value element and return it
     * @return std::pair<node,value>
    */
    std::pair<int, int> removeMinimumValueElement();
    void insert(int node, int value);
    bool empty();
    bool isIn(int node);
    std::map<int, int> nodeIndexedStore;
    std::multimap<int, int> valueIndexedStore;
private:
    //std::map<int, int> nodeIndexedStore;
    //std::multimap<int, int> valueIndexedStore;
    void changeElementIndex(const int oldIndex, const int newIndex, const int element);
    void safeErase(int node, int value);
};