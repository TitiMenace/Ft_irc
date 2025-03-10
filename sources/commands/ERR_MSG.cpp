
#include "includes.hpp"
#include "Server.hpp"
#include "parsingUtils.hpp"
#include "Channel.hpp"

void    ERR_CHANNELISFULL(Client &client, Channel &channel){
	std::stringstream	soutput;
    std::string         output;

	std::cerr << "err chann is full called\n";

	soutput << "471";
	soutput << " " << client.nickname;
	soutput << " " << channel.name;
	soutput << " :" << "Cannot join channel (+l)";
	soutput << "\r\n";
    output = soutput.str();

	std::cerr << output;
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_INVITEONLYCHAN(Client &client, Channel &channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "473";
	soutput << " " << client.nickname;
	soutput << " " << channel.name;
	soutput << " :" << "Cannot join channel (+i)";
	soutput << "\r\n";
    output = soutput.str();

    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_BADCHANMASK(Client &client, std::string channel_name){
	std::stringstream	soutput;
    std::string         output;

	soutput << "475";
	soutput << " " << channel_name;
	soutput << " :" << "Bad Channel Mask";
	soutput << "\r\n";
    output = soutput.str();

    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_BADCHANNELKEY(Client &client, Channel &channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "475";
	soutput << " " << client.nickname;
	soutput << " " << channel.name;
	soutput << " :" << "Cannot join channel (+k)";
	soutput << "\r\n";
    output = soutput.str();

    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_UMODEUNKNOWNFLAG(Client &client, char c){
	std::stringstream	soutput;
    std::string         output;

	soutput << "501";
	soutput << " " << client.nickname;
	soutput << " :" << "Unknown MODE flag (" << c << ")";
	soutput << "\r\n";
    output = soutput.str();

    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}
void    ERR_NOSUCHCHANNEL(Client &client, std::string channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "403";
	soutput << " " << client.nickname;
	soutput << " " << channel;
	soutput << " :" << "No such channel";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_CHANOPRIVSNEEDED(Client &client, std::string channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "482";
	soutput << " " << client.nickname;
	soutput << " " << channel;
	soutput << " :" << "You're not channel operator";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_NOTONCHANNEL(Client &client, std::string channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "442";
	soutput << " " << client.nickname;
	soutput << " " << channel;
	soutput << " :" << "You're not on that channel";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_NOSUCHNICK(Client &client, std::string nickname){
	std::stringstream	soutput;
    std::string         output;

	soutput << "401";
	soutput << " " << client.nickname << " " << nickname;
	soutput << " :" << "No such nick/channel";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}


void    ERR_USERONCHANNEL(Client &client, std::string channel, std::string nick){
	std::stringstream	soutput;
    std::string         output;

	soutput << "443";
	soutput << " " << client.nickname;
    soutput << " " << nick;
	soutput << " " << channel;
	soutput << " :" << "is already on channel";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_NEEDMOREPARAMS(Client &client){
	std::stringstream	soutput;
    std::string         output;

	soutput << "461";
	soutput << " " << client.nickname;
	soutput << " :" << "Not enough parameters";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_USERSDONTMATCH(Client &client){
	std::stringstream	soutput;
    std::string         output;

	soutput << "502";
	soutput << " " << client.nickname;
	soutput << " :" << "Cant change mode for other users";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_ALREADYREGISTERED(Client &client){
	std::stringstream	soutput;
    std::string         output;

	soutput << "462";
	soutput << " " << client.nickname;
	soutput << " :" << "You may not reregister";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_NORECIPIENT(Client &client, std::string nickname, Message &message){
	std::stringstream	soutput;
    std::string         output;

	soutput << "411";
	soutput << " " << nickname;
	soutput << " :" << "No recipient given (" << message.command << ")";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_NOTEXTTOSEND(Client &client, std::string nickname){
	std::stringstream	soutput;
    std::string         output;

	soutput << "412";
	soutput << " " << nickname;
	soutput << " :" << "No text to send";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    ERR_NOTREGISTERED(Client &client){
	std::stringstream	soutput;
    std::string         output;

	soutput << "451";
	soutput << " " << client.nickname;
	soutput << " :" << "You have not registered";
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}