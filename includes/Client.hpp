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
	std::string	inBuffer;
	std::string	outBuffer;
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;
};

