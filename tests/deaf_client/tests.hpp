#pragma once

#include <sys/types.h>
#include <stdexcept>

pid_t	launch_server(uint16_t port, const std::string &password) throw (std::runtime_error);
void	stop_server(pid_t server_pid) throw(std::runtime_error);
int		connect_to_server(uint16_t port);
