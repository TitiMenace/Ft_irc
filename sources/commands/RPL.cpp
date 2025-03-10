
#include "includes.hpp"

#include "RPL.hpp"

void RPL_WELCOME(Client &client) {
	//RPL 001
	dprintf(client.socket_fd, "001 %s :Welcome to the WiZ insane chat of distortion of reality between worlds, %s!%s@%s\r\n", client.nickname.c_str(), client.nickname.c_str(), client.username.c_str(), client.hostname.c_str());
	//RPL 002
	dprintf(client.socket_fd, "002 %s :Your host is %s, running version %s\r\n", client.nickname.c_str(), client.servername.c_str(), "v.1" );
	//RPL 003
	dprintf(client.socket_fd, "003 %s :This server was created %s\r\n", client.nickname.c_str(), "le 01/01/01");
	//RPL 004
	dprintf(client.socket_fd, "004 %s :%s %s %s %s\r\n", client.nickname.c_str(), client.servername.c_str(), "v.1", "no user mode support", "+tlkoiq");
}