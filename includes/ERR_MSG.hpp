#pragma once
#include "includes.hpp"

void    ERR_CHANNELISFULL(Client &client, Channel &channel);
void    ERR_INVITEONLYCHAN(Client &client, Channel &channel);
void    ERR_BADCHANNELKEY(Client &client, Channel &channel);
void    ERR_UMODEUNKNOWNFLAG(Client &client, std::string flag);
void    ERR_UNKNOWNMODE(Client &client, std::string flag);

void    ERR_NOSUCHCHANNEL(Client &client, std::string channel);
void    ERR_CHANOPRIVSNEEDED(Client &client, std::string channel);
void	ERR_INVALIDKEY(Client &client, std::string channel);
void	ERR_INVALIDMODEPARAM(Client &client, std::string channel, std::string flag, std::string param, std::string message);

void    ERR_NOTONCHANNEL(Client &client, std::string channel);
void    ERR_NOSUCHNICK(Client &client, std::string nickname);
void    ERR_USERONCHANNEL(Client &client, std::string channel, std::string nick);

void    ERR_NEEDMOREPARAMS(Client &client, std::string command, std::string message = "Not enough parameters");
void    ERR_USERSDONTMATCH(Client &client);
void    ERR_ALREADYREGISTERED(Client &client);
void    ERR_NORECIPIENT(Client &client, Message &message);

void    ERR_NOTEXTTOSEND(Client &client);
void    ERR_NOTREGISTERED(Client &client);

void    ERR_BADCHANMASK(Client &client, std::string channel_name);

void    ERR_ERRONEUSNICKNAME(Client &client, std::string nickname);
void    ERR_NONICKNAMEGIVEN(Client &client);
void    ERR_NICKNAMEINUSE(Client &client, std::string nickname);

void    ERR_ALREADYREGISTERED(Client &client);
void    ERR_PASSWDMISMATCH(Client &client);

void	ERR_USERNOTINCHANNEL(Client &client, std::string nickname, std::string channel_name);