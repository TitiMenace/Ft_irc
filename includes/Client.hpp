#pragma once

#include "includes.hpp"

struct Client{
	int			socket_fd;
	std::string	nickname;
	std::string	hostname;
	std::string	username;
};

