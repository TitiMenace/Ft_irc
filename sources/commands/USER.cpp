
#include "includes.hpp"
#include "Server.hpp"

#define USRLEN 16

void Server::user(Message message, Client &client){
	std::cout << "Command USER starting" << std::endl;
	if (client.username != ""){
		dprintf(client.socket_fd,"462 ERR_ALREADYREGISTERED\r\n");
		return;
	}
	if (message.params.size() != 4)
	{
		dprintf(client.socket_fd, "461 ERR_NEEDMOREPARAMS\r\n");
		return;
	}
	client.username = message.params[0].substr(0, USRLEN);	
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

		//RPL 001
		dprintf(client.socket_fd, "001 %s :Welcome to the WiZ insane chat of distortion of reality between worlds, %s!%s@%s\n", client.nickname.c_str(), client.nickname.c_str(), client.username.c_str(), client.hostname.c_str());
		//RPL 002
		dprintf(client.socket_fd, "002 %s :Your host is %s, running version %s\r\n", client.nickname.c_str(), client.servername.c_str(), "v.1" );
		//RPL 003
		dprintf(client.socket_fd, "003 %s :This server was created %s\r\n", client.nickname.c_str(), "le 01/01/01");
		//RPL 004
		dprintf(client.socket_fd, "004 %s :%s %s %s %s\r\n", client.nickname.c_str(), client.servername.c_str(), "v.1", "fil with user modes", "fill with channel modes");
	}
	return;
}

