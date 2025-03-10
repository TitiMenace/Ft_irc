#pragma once
#include "includes.hpp"

#include <map>
#include "Client.hpp"
#include "Message.hpp"
#define SIZE_LIMIT 10

enum ChannelMode {
	NO_FLAG,
	INVITE_ONLY     = 1 << 0,   // 00001
	TOPIC_ONLY_OP   = 1 << 1,   // 00010
	KEY_PROTECTED   = 1 << 2,   // 00100
	USER_LIMIT		= 1 << 3    // 01000
};

// string name
// string topic
// map<client_socket, Client> list_user
// map<client_socket, Client> list_operator
// map<client_socket, Client> list_invite
// enum mode
struct Channel{
	std::string           name;
    std::string           topic;
    std::string           key;
    size_t                   size_limit;
    std::map<int, Client> list_user;
    std::map<int, Client> list_operator;
    std::map<int, Client> list_invite;
    int mode;
	
    Channel(std::string name = "", std::string topic = "", std::string key = "", int size_limit = 0, ChannelMode mode = NO_FLAG);
};

std::string getchannelmodes(Channel channel);
std::string getchannelmodarg(Channel channel);



void    ERR_CHANNELISFULL(Client &client, Channel &channel);
void    ERR_INVITEONLYCHAN(Client &client, Channel &channel);
void    ERR_BADCHANNELKEY(Client &client, Channel &channel);
void    ERR_UMODEUNKNOWNFLAG(Client &client, char c);

void    ERR_NOSUCHCHANNEL(Client &client, std::string channel);
void    ERR_CHANOPRIVSNEEDED(Client &client, std::string channel);
    
void    ERR_NOTONCHANNEL(Client &client, std::string channel);
void    ERR_NOSUCHNICK(Client &client, std::string nickname);
void    ERR_USERONCHANNEL(Client &client, std::string channel, std::string nick);

void    ERR_NEEDMOREPARAMS(Client &client);
void    ERR_USERSDONTMATCH(Client &client);
void    ERR_ALREADYREGISTERED(Client &client);
void    ERR_NORECIPIENT(Client &client, std::string nickname, Message &message);

void    ERR_NOTEXTTOSEND(Client &client, std::string nickname);
void    ERR_NOTREGISTERED(Client &client);

void    ERR_BADCHANMASK(Client &client, std::string channel_name);