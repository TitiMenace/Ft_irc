
#include "includes.hpp"
#include "Server.hpp"

void Server::ping(Message message, Client &client){
	if (message.params.size() < 1) {
		ERR_NEEDMOREPARAMS(client, message);
		return;
	}
    std::stringstream output;
	output << "PONG " << "server" << " :" << message.params[0];
    output << "\r\n";
    client.outBuffer += output.str();
    return;
}

