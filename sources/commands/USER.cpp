
#include "includes.hpp"

#include "RPL.hpp"

#include "Server.hpp"

#define USRLEN 16

void Server::user(Message message, Client &client){
	std::cout << "Command USER starting" << std::endl;
	if (client.state == REGISTERED) {
		ERR_ALREADYREGISTERED(client);
		return;
	}
	if (message.params.size() != 4) {
		ERR_NEEDMOREPARAMS(client);
		return;
	}

	if (message.params[1] != "0" && message.params[1] != "*")
		client.hostname = message.params[1];
	if (message.params[2] != "0" && message.params[2] != "*")
		client.servername = message.params[2];
	client.username = message.params[0].substr(0, USRLEN);
	client.realname = message.params[3];

	std::cout << "====USER COMMAND DONE======\n\n" << std::endl;
	std::cout << "For Client (" << client.socket_fd << ")\n\n";
	std::cout << "username : "<< client.username << std::endl;
	std::cout << "hostname : "<< client.hostname << std::endl;
	std::cout << "servername: "<< client.servername << std::endl;
	std::cout << "realname: "<< client.realname << std::endl;

	if (!client.nickname.empty()){
		client.state = REGISTERED;//send RPL_WELCOME and stuff
		RPL_WELCOME(client);
	}
	return;
}

