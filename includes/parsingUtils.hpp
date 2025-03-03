#pragma once
#include <map>
bool matchChar(std::string &buffer, std::size_t &buffer_pos, char c);

bool charInSet(char c, std::string set);

bool parseSpaces(std::string &buffer, std::size_t &buffer_pos);

template <typename K, typename V>
V* findInMap(std::map<K, V>& myMap, K& key) {
    typename std::map<K, V>::iterator it = myMap.find(key);
    if (it != myMap.end()) {
        return &(it->second);
    }
    return NULL;
}