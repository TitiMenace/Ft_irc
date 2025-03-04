
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