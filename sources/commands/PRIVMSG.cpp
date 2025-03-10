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
	
	std::string nickname = client.nickname.empty() ? "*" : client.nickname;

	if (message.params.size() == 0) {
		ERR_NORECIPIENT(client,nickname, message);
		return;
	}
	if (message.params.size() == 1) {
		ERR_NOTEXTTOSEND(client, nickname);
		return;
	}
	if (client.state != REGISTERED) {
		ERR_NOTREGISTERED(client);
		return;
	}

	std::string &target =  message.params[0];
	std::string text = message.params[1];


	if (target[0] == '#'){
	
		for (std::map<std::string, Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++){
			if (it->first == target){
				channelMessage(_channel_list[it->first], text, client);
			}
		}
	}
	else {
		
		for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++) {
			if (it->second.nickname == target) {
				std::cout << "on entre bien dans la boucle et " << client.nickname << " envoie bien un message a " << target << " : " << text << std::endl;
				dprintf(it->first, ":%s PRIVMSG %s :%s\r\n", client.nickname.c_str(), target.c_str(), text.c_str());
				
			}
		}
		ERR_NOSUCHNICK(client, "");
	}
}

