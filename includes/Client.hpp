#pragma once
#include "includes.hpp"
enum UserState {
	ANONYMOUS,
	ALLOWED,
	REGISTERED
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

