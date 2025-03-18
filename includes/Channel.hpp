#pragma once
#include "includes.hpp"

#include <set>
#include "Client.hpp"
#include "Message.hpp"
#define SIZE_LIMIT 10

enum ChannelMode {
	NO_FLAG,
	INVITE_ONLY     = 1 << 0,   // 00001
	TOPIC_ONLY_OP   = 1 << 1,   // 00010
	KEY_PROTECTED   = 1 << 2,   // 00100
	USER_LIMIT		= 1 << 3    // 01000
};

struct Channel{
	std::string		name;
	std::string		topic;
	std::string		key;
	size_t			size_limit;
	std::set<int>	members;
	std::set<int>	operators;
	std::set<int>	invites;
	int mode;
	
    Channel(std::string name = "", std::string topic = "", std::string key = "", int size_limit = 0, ChannelMode mode = NO_FLAG);
};

std::string getchannelmodes(Channel channel);
std::string getchannelmodarg(Channel channel);

