
#include "includes.hpp"
#include "RPL.hpp"

void RPL_WELCOME(Client &client) {
    std::stringstream output;

	output << "001 " << client.nickname <<
        " :Welcome to the WiZ insane chat of distortion of reality between worlds, " <<
        client.nickname << "!" << client.nickname << "@" << client.username;
    output << "\r\n";
	output << "002 " << client.nickname << " :Your host is " << 
        client.servername.c_str() << ", running version " << "v.1";
    output << "\r\n";
	output << "003 " << client.nickname << " :This server was created " << 
    "le 01/01/01";
    output << "\r\n";	
	output << "004 " << client.nickname << " :" << client.servername
     << "v.1" << " " << "no user mode support" << " " << "+tlkoiq";
    output << "\r\n";
    client.outBuffer += output.str();;
}

void RPL_TOPIC(Channel &channel, Client &client) {
    std::stringstream soutput;
    
    soutput << "332 " << client.nickname << " " << 
        channel.name << " :" << channel.topic;
    soutput << "\r\n";
    client.outBuffer += soutput.str();
}

void RPL_NOTOPIC(Channel &channel, Client &client) {
    std::stringstream output;
    
    output << "331 " << client.nickname << " " <<
        channel.name << " :No topic is set";
    output << "\r\n";
    client.outBuffer += output.str();
}

void RPL_CHANNELMODEIS(Channel &channel, Client &client){
    std::stringstream output;
    std::string modes = getchannelmodes(channel);
    std::string modargs = getchannelmodarg(channel);
    output <<  "324 " << client.nickname << " " << channel.name << " :" <<
    modes <<" " << modargs;
    output << "\r\n";
    client.outBuffer += output.str();
}

void RPL_KICK(Channel &channel, Client &client, std::string kicked, std::string source, std::string comment) {
    std::stringstream output;
    
    output << ":" << source << " KICK " << channel.name << " " << kicked;
	output << " :" << (comment.empty() ? "No particular reason." : comment);
    output << "\r\n";
    client.outBuffer += output.str();
}
void    RPL_INVITE(Client &client, std::string channel, std::string nick){
	std::stringstream	output;
    std::string         source;
    
    output << ":" << nick;
	output << " " << "INVITE";
    output << " " << nick;
	output << " " << client.nickname;
	output << " " << channel;
	output << "\r\n";
    client.outBuffer += output.str();
	return;
}
void    RPL_INVITING(Client &client, std::string channel, std::string nick){
	std::stringstream	output;

	output << "341";
	output << " " << client.nickname;
    output << " " << nick;
	output << " " << channel;
	output << " :" << "Inviting " << nick << " in channel " << channel;
	output << "\r\n";
    client.outBuffer += output.str();
	return;
}

void    RPL_ENDOFNAMES(Client &client, Channel &channel){
    std::stringstream	output;
    std::string cname = channel.name;


	output << "366 " << client.nickname;
	output << " " << cname <<  " :End of /NAMES list"; 
    output << "\r\n";
    client.outBuffer += output.str();
	return;
} 

void    RPL_NAMREPLY(Client &client, Channel &channel, std::map<int, Client> &users){
    std::stringstream	soutput;
    std::string cname = channel.name;
    std::string output;

	soutput << "353 " << client.nickname;
	soutput << " = " << cname <<  " :";
    for (std::set<int>::iterator it = channel.members.begin(); it != channel.members.end(); it++){
        if (channel.operators.count(*it)){
            soutput << "@";
        }
	    soutput << users[*it].nickname << " ";
    }
    output = soutput.str();
    output.erase(output.size() - 1);
    output += "\r\n";
    client.outBuffer += output;
	return;
}



void    RPL_INVEXLIST(Client &client, Channel &channel){
    std::stringstream	output;
    std::string cname = channel.name;

	output << "346 " << client.nickname << " ";
	output << channel.name << " ";
    output << "INVITE_ONLY";
    output << "\r\n";
    client.outBuffer += output.str();
	return;
}

void    RPL_ENDOFINVEXLIST(Client &client, Channel &channel){
    std::stringstream	output;
    std::string cname = channel.name;

	output << "347 " << client.nickname << " ";
	output << channel.name << " :";
    output << "End of Channel Invite Exception List";
    output << "\r\n";
    client.outBuffer += output.str();
	return;
}

void    RPL_JOIN(Client &client, std::string channel_name){
    std::stringstream	output;
    std::string cname = channel_name;//.substr(1, channel_name.size());
    
    output << ":" << client.nickname;
    output << "!" << client.username;
    output << "@" << client.hostname; 
    output << " JOIN " << cname;
    output << "\r\n";
    client.outBuffer += output.str();
    return;
}

void RPL_PRIVMSG(Client &client, std::string source, std::string target, std::string message){
    std::stringstream output;
	output << ":" << source << 
	" PRIVMSG " << target << " :" << message;
	output << "\r\n";
    client.outBuffer += output.str();
}