
#include "Channel.hpp"
#include "parsingUtils.hpp"


Channel::Channel(std::string name, std::string topic, std::string key, int size_limit, ChannelMode mode)
    : name(name), topic(topic), key(key), size_limit(size_limit), mode(mode) {
	}

    // tkIlo

std::string getchannelmodes(Channel channel){

    std::stringstream output;

    if (channel.mode & TOPIC_ONLY_OP)
        output << "t";
    if (channel.mode & KEY_PROTECTED)
        output << "k";
    if (channel.mode & INVITE_ONLY)
        output << "I";
    if (channel.mode & USER_LIMIT)
        output << "l";
    if (!channel.operators.empty())
        output << "o";

    return (output.str());    
}

std::string getchannelmodarg(Channel channel){

    std::stringstream   output;
    
    if (channel.mode & TOPIC_ONLY_OP)
        output << channel.topic << " ";
    if (channel.mode & KEY_PROTECTED)
        output << channel.key << " ";
    if (channel.mode & USER_LIMIT)
        output << channel.size_limit;

    return (output.str());
}

