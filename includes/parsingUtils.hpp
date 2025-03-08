#pragma once
#include <map>

bool matchChar(std::string &buffer, std::size_t &buffer_pos, char c);

bool charInSet(char c, std::string set);

bool parseSpaces(std::string &buffer, std::size_t &buffer_pos);


bool parseChanString(std::string &buffer, std::size_t &buffer_pos);
bool parseChannelID(std::string &buffer, std::size_t &buffer_pos);
bool parseChannel(std::string &buffer, std::size_t &buffer_pos);
bool parseChannelList(std::string buffer, std::vector<std::string> &out_list);
bool parseKeyList(std::string buffer, std::vector<std::string> &out_list);

template <typename K, typename V>
V* findInMap(std::map<K, V>& myMap, K& key) {
    typename std::map<K, V>::iterator it = myMap.find(key);
    if (it != myMap.end()) {
        return &(it->second);
    }
    return NULL;
}