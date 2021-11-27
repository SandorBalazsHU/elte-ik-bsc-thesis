#pragma once
#include <vector>
#include <map>
#include <set>

class reverseMap {
public:
    reverseMap() {}
    ~reverseMap() {}
    int getValueByNode(size_t node);
    void setValueByNode(size_t node, int newValue);
    /**
     * @brief Remove the minimum value element and return it
     * @return std::pair<node,value>
    */
    std::pair<size_t, int> removeMinimumValueElement();
    void insert(size_t node, int value);
    bool empty();
    bool isIn(size_t node);
    std::map<size_t, int> nodeIndexedStore;
    std::multimap<int, size_t> valueIndexedStore;
private:
    //std::map<int, int> nodeIndexedStore;
    //std::multimap<int, int> valueIndexedStore;
    void changeElementIndex(const size_t oldIndex, const size_t newIndex, const int element);
    void safeErase(size_t node, int value);
};