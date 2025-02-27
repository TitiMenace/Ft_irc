#include "Server.hpp"
#include <stdio.h>

// Can send errors:
// - ERR_NORECIPIENT (411)
// - ERR_NOTEXTTOSEND (412)
// - ERR_NOTREGISTERED (451)
void Server::privmsg(Message message, Client &client) {
	const char *nickname = client.nickname.empty() ? "*" : client.nickname.c_str();

	if (message.params.size() == 0) {
		dprintf(client.socket_fd, ":localhost 411 %s :No recipient given (%s)\r\n", nickname, message.command.c_str());
		return;
	}
	if (message.params.size() == 1) {
		dprintf(client.socket_fd, ":localhost 412 %s :No text to send\r\n", nickname);
		return;
	}

	// What should we do when there are more than two params?
	
	if (client.nickname.empty() || client.username.empty()) {
		dprintf(client.socket_fd, ":localhost 451 %s :You have not registered\r\n", client.nickname.c_str());
		return;
	}

	std::string &target = message.params[1];
	std::string &text = message.params[1];
	for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->first != client.socket_fd) {
			dprintf(it->first, ":localhost PRIVMSG %s %s\r\n", target.c_str(), text.c_str());
		}
	}

}
