#pragma once

#include "Server.hpp"

void    RPL_WELCOME(Client &client);void RPL_WELCOME(Client &client);
void    RPL_JOIN(Client &client, std::string channel_name, Client &member);
void    RPL_TOPIC(Channel &channel, Client &client);
void    RPL_NOTOPIC(Channel &channel, Client &client);
void    RPL_CHANNELMODEIS(Channel &channel, Client &client);
void    RPL_KICK(Channel &channel, Client &client, std::string kicked, std::string source, std::string comment = "");
void    RPL_INVITE(Client &client, std::string channel, std::string nick);
void    RPL_INVITING(Client &client, std::string channel, std::string nick);
void    RPL_ENDOFNAMES(Client &client, Channel &channel);
void    RPL_INVEXLIST(Client &client, Channel &channel);
void    RPL_ENDOFINVEXLIST(Client &client, Channel &channel);
void    RPL_NAMREPLY(Client &client, Channel &channel, std::map<int, Client> &users);
void    RPL_PRIVMSG(Client &client, std::string source, std::string target, std::string message);
