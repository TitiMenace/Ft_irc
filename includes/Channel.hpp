#pragma once
#include "includes.hpp"

#include "Client.hpp"
enum ChannelMode {
	INVITE_ONLY     = 1 << 0,   // 00001
	TOPIC_ONLY_OP   = 1 << 1,   // 00010
	KEY_PROTECTED   = 1 << 2,   // 00100
	USER_LIMIT		= 1 << 3    // 01000
};

// string name
// string topic
// map<client_socket, Client> list_user
// map<client_socket, Client> list_operator
// map<client_socket, Client> list_invite
// enum mode
struct Channel{
	std::string           name;
    std::string           topic;
    int                   size_limit;
    std::map<int, Client> list_user;
    std::map<int, Client> list_operator;
    std::map<int, Client> list_invite;
    ChannelMode mode;
};

