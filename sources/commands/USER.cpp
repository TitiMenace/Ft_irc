
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
	Client 

    return;
}

