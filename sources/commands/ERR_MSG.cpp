
#include "includes.hpp"
#include "Server.hpp"
#include "parsingUtils.hpp"
#include "Channel.hpp"

void    ERR_CHANNELISFULL(Client &client, Channel &channel){
	std::stringstream	output;

	std::cerr << "err chann is full called\n";

	output << "471";
	output << " " << client.nickname;
	output << " " << channel.name;
	output << " :" << "Cannot join channel (+l)";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_INVITEONLYCHAN(Client &client, Channel &channel){
	std::stringstream	output;

	output << "473";
	output << " " << client.nickname;
	output << " " << channel.name;
	output << " :" << "Cannot join channel (+i)";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_BADCHANMASK(Client &client, std::string channel_name){
	std::stringstream	output;

	output << "476";
	output << " " << channel_name;
	output << " :" << "Bad Channel Mask";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_BADCHANNELKEY(Client &client, Channel &channel){
	std::stringstream	output;

	output << "475";
	output << " " << client.nickname;
	output << " " << channel.name;
	output << " :" << "Cannot join channel (+k)";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_UMODEUNKNOWNFLAG(Client &client, std::string flag){
	std::stringstream	output;

	output << "501";
	output << " " << client.nickname;
	output << " :" << "Unknown MODE flag (" << flag << ")";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_UNKNOWNMODE(Client &client, std::string flag){
	std::stringstream	output;

	output << "472";
	output << " " << client.nickname;
	output << " " << flag;
	output << " :is unknown mode char to me";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_NOSUCHCHANNEL(Client &client, std::string channel){
	std::stringstream	output;

	output << "403";
	output << " " << client.nickname;
	output << " " << channel;
	output << " :" << "No such channel";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_CHANOPRIVSNEEDED(Client &client, std::string channel){
	std::stringstream	output;

	output << "482";
	output << " " << client.nickname;
	output << " " << channel;
	output << " :" << "You're not channel operator";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void	ERR_INVALIDKEY(Client &client, std::string channel) {
	std::stringstream	output;

	output << "525";
	output << " " << client.nickname;
	output << " " << channel;
	output << " :" << "Key is not well-formed";
	output << "\r\n";
	client.outBuffer += output.str();
}

void	ERR_INVALIDMODEPARAM(Client &client, std::string channel, std::string flag, std::string param, std::string message) {
	std::stringstream	output;

	output << "696";
	output << " " << client.nickname;
	output << " " << channel;
	output << " " << flag;
	output << " " << param;
	output << " :" << message;
	output << "\r\n";
	client.outBuffer += output.str();
}

void    ERR_NOTONCHANNEL(Client &client, std::string channel){
	std::stringstream	output;

	output << "442";
	output << " " << client.nickname;
	output << " " << channel;
	output << " :" << "You're not on that channel";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_NOSUCHNICK(Client &client, std::string nickname){
	std::stringstream	output;

	output << "401";
	output << " " << client.nickname << " " << nickname;
	output << " :" << "No such nick/channel";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}


void    ERR_USERONCHANNEL(Client &client, std::string channel, std::string nick){
	std::stringstream	output;

	output << "443";
	output << " " << client.nickname;
    output << " " << nick;
	output << " " << channel;
	output << " :" << "is already on channel";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_NEEDMOREPARAMS(Client &client, std::string command, std::string message){
	std::stringstream	output;

	output << "461";
	output << " " << (client.nickname.empty() ? "*" : client.nickname);
	output << " " << command;
	output << " :" << message;
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_USERSDONTMATCH(Client &client){
	std::stringstream	output;

	output << "502";
	output << " " << client.nickname;
	output << " :" << "Cant change mode for other users";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_ALREADYREGISTERED(Client &client){
	std::stringstream	output;

	output << "462";
	output << " " << client.nickname;
	output << " :" << "You may not reregister";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_NORECIPIENT(Client &client, Message &message){
	std::stringstream	output;

	output << "411";
	output << " " << client.nickname;
	output << " :" << "No recipient given (" << message.command << ")";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_NOTEXTTOSEND(Client &client){
	std::stringstream	output;

	output << "412";
	output << " " << client.nickname;
	output << " :" << "No text to send";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void    ERR_NOTREGISTERED(Client &client){
	std::stringstream	output;

	output << "451 ";
	output << (client.nickname.empty() ? "*" : client.nickname);
	output << " :" << "You have not registered";
	output << "\r\n";
	client.outBuffer += output.str();
	return;
}

void ERR_ERRONEUSNICKNAME(Client &client, std::string nickname) {
    std::stringstream output;
    
    output << "432 ";
    output << (client.nickname.empty() ? "*" : client.nickname);
	output << " " << nickname;
	output << " :Erroneus nickname";
    output << "\r\n";
    client.outBuffer += output.str();
}

void ERR_NONICKNAMEGIVEN(Client &client) {
    std::stringstream output;
    
    output << "461 ";
    output << (client.nickname.empty() ? "*" : client.nickname);
	output << " :No nickname given";
    output << "\r\n";
    client.outBuffer += output.str();
}

void ERR_NICKNAMEINUSE(Client &client, std::string nickname) {
    std::stringstream output;
    
    output << "433 ";
    output << (client.nickname.empty() ? "*" : client.nickname);
    output << " " << nickname;
    output << " :Nickname is already in use.";
    output << "\r\n";
    client.outBuffer += output.str();
}

void ERR_PASSWDMISMATCH(Client &client) {
    std::stringstream output;
    
    output << "464 " << (client.nickname.empty() ? "*" : client.nickname) << " :Password incorrect";
    output << "\r\n";
    client.outBuffer += output.str();
}

void ERR_USERNOTINCHANNEL(Client &client, std::string nickname, std::string channel_name) {
	std::stringstream output;

	output << "441";
	output << " " << client.nickname;
	output << " " << nickname;
	output << " " << channel_name;
	output << " :They aren't on that channel\r\n";
	client.outBuffer += output.str();
}
