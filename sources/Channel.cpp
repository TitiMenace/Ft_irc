
#include "Channel.hpp"
#include "parsingUtils.hpp"


Channel::Channel(std::string name, std::string topic, std::string key, int size_limit, ChannelMode mode)
    : name(name), topic(topic), key(key), size_limit(size_limit), mode(mode) {
	}

