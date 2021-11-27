#include "reverseMap.h"

int reverseMap::getValueByNode(size_t node) {
    return nodeIndexedStore[node];
}
void reverseMap::setValueByNode(size_t node, int newValue) {
    int oldValue = nodeIndexedStore[node];
    nodeIndexedStore[node] = newValue;
    changeElementIndex(oldValue, newValue, node);
}

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

void reverseMap::insert(size_t node, int value) {
    nodeIndexedStore.insert(std::pair<size_t, int>(node, value));
    valueIndexedStore.insert(std::pair<int, size_t>(value, node));
}

bool reverseMap::empty() {
    return nodeIndexedStore.empty() && valueIndexedStore.empty();
}

bool reverseMap::isIn(size_t node) {
    return nodeIndexedStore.find(node) != nodeIndexedStore.end();
}