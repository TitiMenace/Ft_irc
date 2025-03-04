#include "Server.hpp"
#include <stdio.h>

// Can send errors:
// - ERR_NORECIPIENT (411)
// - ERR_NOTEXTTOSEND (412)
// - ERR_NOTREGISTERED (451)

void	channelMessage(Channel &channel, std::string output, Client &client){
	
	std::cout << "on ecrit a tout le monde dans le channel" << std::endl;
	for (std::map<int, Client>::iterator it = channel.list_user.begin(); it != channel.list_user.end(); it++){

		if (client.socket_fd != it->first)
			dprintf(it->first, ":%s PRIVMSG %s :%s\r\n",client.nickname.c_str(), channel.name.c_str(), output.c_str());
	}
	
}


void Server::privmsg(Message message, Client &client) {
	
	std::cout << "debut de la commande privmsg" << std::endl;
	
	const char *nickname = client.nickname.empty() ? "*" : client.nickname.c_str();

	if (message.params.size() == 0) {
		dprintf(client.socket_fd, ":localhost 411 %s :No recipient given (%s)\r\n", nickname, message.command.c_str());
		return;
	}
	if (message.params.size() == 1) {
		dprintf(client.socket_fd, ":localhost 412 %s :No text to send\r\n", nickname);
		return;
	}
	if (client.state != REGISTERED) {
		dprintf(client.socket_fd, ":localhost 451 %s :You have not registered\r\n", client.nickname.c_str());
		return;
	}

	std::string &target =  message.params[0];
	std::string text = message.params[1];
	/*for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->first != client.socket_fd) {
			dprintf(it->first, ":localhost PRIVMSG %s %s\r\n", target.c_str(), text.c_str());
		}
	}*/
	
	for (std::map<std::string, Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++){
		if (it->first == target){
			channelMessage(_channel_list[it->first], text, client);
		}

	}
	

	
}

