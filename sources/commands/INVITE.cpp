#include "Server.hpp"
#include "Channel.hpp"
#include "parsingUtils.hpp"


void    ERR_NOSUCHCHANNEL(Client &client, std::string channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "461";
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

void    ERR_NOSUCHNICK(Client &client, std::string channel){
	std::stringstream	soutput;
    std::string         output;

	soutput << "401";
	soutput << " " << client.nickname;
	soutput << " " << channel;
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
void Server::invite(Message message, Client &client) {
std::cout << "invite command starting"<< std::endl;
    if (message.params.size() < 2){ 
        std::cout << "not enough params"<< std::endl;
        return; // ERR_NEEDMOREPARAMS (461)
    }
    std::string nickname = message.params[0];
    std::string channel_name = message.params[1];
    std::cout << "invite command with nickname " << nickname << "and channel_name" << channel_name << std::endl;
    
    if (!findInMap(_channel_list, channel_name)){
        std::cout << "channel dont exist" << std::endl;
        ERR_NOSUCHCHANNEL(client, channel_name);
        return;
    }

    Channel &channel = _channel_list[channel_name];
    std::cout << "channel "<< channel.name << "exists" << std::endl;

    if (!findInMap(channel.list_operator, client.socket_fd) && channel.mode & INVITE_ONLY){
        std::cout << "Needs to be operator to invite in invite only" << std::endl;
        ERR_CHANOPRIVSNEEDED(client, channel_name);
        return; //  ERR_CHANOPRIVSNEEDED (482) 
    }

    if (!findInMap(channel.list_user, client.socket_fd)){
        std::cout << "need to be in the channel to invite someone in it" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    Client* invited = NULL;
 
    for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (it->second.nickname == nickname) {
            invited = &(it->second);
            break;
        }
    }

    if (invited == NULL) {
        std::cout << "user to invite doesnt exists" << std::endl;
        ERR_NOSUCHNICK(client, channel_name);
        return;// ERR_NOSUCHNICK (401)
    }

    if (findInMap(channel.list_user, invited->socket_fd)){
        std::cout << "user to invite already in the channel" << std::endl;
        ERR_USERONCHANNEL(client, channel_name, invited->nickname);
        return; // ERR_USERONCHANNEL (443)
    }

    // Continue with normal invite logic using 'invited'
    channel.list_invite[invited->socket_fd] = *invited;
        
    std::cout << "invite list:" << std::endl;
    for (std::map<int, Client>::iterator it = channel.list_invite.begin(); it != channel.list_invite.end(); ++it) {
        std::cout << it->second.nickname << std::endl;
    }

    // RPL_INVITING (341)
    
// When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer
// , and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

    RPL_INVITING(client, channel_name, invited->nickname);
    RPL_INVITE(*invited, channel_name, client.nickname);
    return;
}
/*irssi -c localhost -p 8080 -w popo

The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>.

The target channel SHOULD exist (at least one user is on it). Otherwise, the server SHOULD reject the command with the ERR_NOSUCHCHANNEL numeric.

Only members of the channel are allowed to invite other users. Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.

Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator.

If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.

When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

*/
