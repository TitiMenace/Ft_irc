
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
void RPL_TOPIC(Channel &channel, Client &client) {
    std::stringstream soutput;
    std::string output;
    
    soutput << "332 " << client.nickname << " " << channel.name << " :" << channel.topic << "\r\n";
    output = soutput.str();
    
    dprintf(client.socket_fd, "%s", output.c_str());
}

void RPL_NOTOPIC(Channel &channel, Client &client) {
    std::stringstream soutput;
    std::string output;
    
    soutput << "331 " << client.nickname << " " << channel.name << " :No topic is set\r\n";
    output = soutput.str();
    
    dprintf(client.socket_fd, "%s", output.c_str());
}

void RPL_CHANNELMODEIS(Channel &channel, Client &client){

    std::string modes = getchannelmodes(channel);
    std::string modargs = getchannelmodarg(channel);
    dprintf(client.socket_fd, "324 %s %s :%s %s...\r\n",client.nickname.c_str(), channel.name.c_str(), modes.c_str(), modargs.c_str());
}

void RPL_KICK(Channel &channel, Client &client, std::string kicked, std::string source, std::string comment) {
    std::stringstream soutput;
    std::string output;
    
    soutput << ":" << source << " KICK " << channel.name << " " << kicked;
    
    if (!comment.empty()) {
        soutput << " :" << comment;
    }
    
    soutput << "\r\n";
    output = soutput.str();
    
    dprintf(client.socket_fd, "%s", output.c_str());
}
void    RPL_INVITE(Client &client, std::string channel, std::string nick){
	std::stringstream	soutput;
    std::string         output;
    std::string         source;
    
    soutput << ":" << nick;
	soutput << " " << "INVITE";
    soutput << " " << nick;
	soutput << " " << client.nickname;
	soutput << " " << channel;
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}
void    RPL_INVITING(Client &client, std::string channel, std::string nick){
	std::stringstream	soutput;
    std::string         output;

	soutput << "341";
	soutput << " " << client.nickname;
    soutput << " " << nick;
	soutput << " " << channel;
	soutput << " :" << "Inviting " << nick << " in channel " << channel;
	soutput << "\r\n";
    output = soutput.str();
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}

void    RPL_ENDOFNAMES(Client &client, Channel &channel){

    dprintf(2, "------------------DEBUG-------------------\n----------------channelname : %s---------------\n", channel.name.c_str());
    std::stringstream	soutput;
    std::string cname = channel.name;//.substr(1, channel.name.size());
    std::string output;
	//soutput << ":localhost ";

	soutput << "366 " << client.nickname;
	//soutput << " 353";

	soutput << " " << cname <<  " :End of /NAMES list"; 
    output = soutput.str();
    output += "\r\n";
     dprintf(2, "%s", output.c_str());
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
} 

void    RPL_NAMREPLY(Client &client, Channel &channel){


    std::stringstream	soutput;

    std::string cname = channel.name;//.substr(1, channel.name.size());
    std::string output;
	//soutput << ":localhost ";

	soutput << "353 " << client.nickname;
	//soutput << " 353";

	soutput << " = " << cname <<  " : ";
    for (std::map<int, Client>::iterator it = channel.list_user.begin(); it != channel.list_user.end(); it++){
        if (channel.list_operator.find(it->first) != channel.list_operator.end()){
            soutput << "@";
        }
	    soutput << it->second.nickname << " ";
    }
    output = soutput.str();
    output.erase(output.size() - 1);
    output += "\r\n";
    dprintf(2, "%s", output.c_str());
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}



void    RPL_INVEXLIST(Client &client, Channel &channel){


    std::stringstream	soutput;

    std::string cname = channel.name;
    std::string output;

	soutput << "346 " << client.nickname << " ";
	soutput << channel.name << " ";
    soutput << "INVITE_ONLY";
    dprintf(2, "%s\n", output.c_str());
    dprintf(client.socket_fd, "%s\r\n", output.c_str());
	return;
}

void    RPL_ENDOFINVEXLIST(Client &client, Channel &channel){


    std::stringstream	soutput;

    std::string cname = channel.name;
    std::string output;

	soutput << "347 " << client.nickname << " ";
	soutput << channel.name << " :";
    soutput << "End of Channel Invite Exception List";
    dprintf(2, "%s\n", output.c_str());
    dprintf(client.socket_fd, "%s\r\n", output.c_str());
	return;
}