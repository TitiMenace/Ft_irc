
#include "includes.hpp"
#include "Server.hpp"

void Server::user(Message message, Client &client){
    std::cout << "Command USER starting" << std::endl;
    if (Client.username != "")
	dprintf(Client.socket_fd,"462 ERR_ALREADYREGISTERED");
    	return;
	if (params.size() != 4)
	{
		dprintf(Client.socket_fd, "461 ERR_NEEDMOREPARAMS");
		return;
	}
	if (message.param[0].size() < 1 || message.param[3].size() < 1){
		dprintf(2, "Error: User command: Missing arguments");
		return;
	}
	Client.username = message.param[0];
	Client.realname = message.param[3];

    return;
}

