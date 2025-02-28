
#include "Channel.hpp"
#include "parsingUtils.hpp"


Channel::Channel(std::string name, std::string topic, std::string password, int size_limit, ChannelMode mode)
    : name(name), topic(topic), password(password), size_limit(size_limit), mode(mode) {
	}

