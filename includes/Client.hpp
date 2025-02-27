#pragma once

#include "includes.hpp"

struct Client{
	int			socket_fd;
	std::string	buffer;
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;


};

