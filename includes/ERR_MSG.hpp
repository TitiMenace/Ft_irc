#pragma once
#include "includes.hpp"

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