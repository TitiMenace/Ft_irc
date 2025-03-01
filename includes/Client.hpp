#pragma once
#include "includes.hpp"
enum UserState {
	ANONYMOUS	= 1 << 0,
	ALLOWED		= 1 << 1,
	REGISTERED	= 1 << 2
};

struct Client{
	int			socket_fd;
	UserState	state;
	std::string	buffer;
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;
};

