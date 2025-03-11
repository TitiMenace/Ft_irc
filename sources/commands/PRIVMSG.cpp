#include "Server.hpp"
#include "parsingUtils.hpp"
#include <stdio.h>
#include "RPL.hpp"
// Can send errors:
// - ERR_NORECIPIENT (411)
// - ERR_NOTEXTTOSEND (412)
// - ERR_NOTREGISTERED (451)

	
void	channelMessage(Channel &channel, std::string output, Client &client){
	
	std::cout << "on ecrit a tout le monde dans le channel" << std::endl;
	for (std::map<int, Client>::iterator it = channel.list_user.begin(); it != channel.list_user.end(); it++){
		if (client.socket_fd != it->first)
			RPL_PRIVMSG(client,client.nickname, channel.name, output);
	}
	
}

Client* Server::findUser(std::string nickname)
{
	for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->second.nickname == nickname) {
			return (&it->second);
		}
	}
	return NULL;
} 

void Server::privmsg(Message message, Client &client) {
	std::cout << "debut de la commande privmsg" << std::endl;
	
	if (client.state != REGISTERED) {
		ERR_NOTREGISTERED(client);
		return;
	}
	
	if (message.params.size() == 0) {
		ERR_NORECIPIENT(client, message);
		return;
	}
	if (message.params.size() == 1) {
		ERR_NOTEXTTOSEND(client);
		return;
	}

	std::string &target_name =  message.params[0];
	std::string text = message.params[1];

	Channel *channel;
	if ((channel = findInMap(_channel_list, target_name)) != NULL){
		channelMessage(*channel, text, client);
		return;
	}
	Client *target;
	if ((target = findUser(target_name)) == NULL){
		ERR_NOSUCHNICK(client, "");
		return;
	}
	if (target->state != REGISTERED){
		ERR_NOSUCHNICK(client, "");
		// whatever error when you send a message to someone not registered
		return;
	}

	RPL_PRIVMSG(*target,client.nickname, target_name, text);

}

