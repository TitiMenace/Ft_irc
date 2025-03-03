#include "Server.hpp"
#include "Channel.hpp"
#include "parsingUtils.hpp"
void Server::invite(Message message, Client &client) {

    if (message.params.size() < 2){
        return; // ERR_NEEDMOREPARAMS (461)
    }
    std::string nickname = message.params[0];
    std::string channel_name = message.params[1];
    
    if (!findInMap(_channel_list, channel_name)){
        return; // ERR_NOSUCHCHANNEL (403)
    }

    Channel &channel = _channel_list[channel_name];

    if (!findInMap(channel.list_operator, client.socket_fd) && channel.mode & INVITE_ONLY){
        return; //  ERR_CHANOPRIVSNEEDED (482) 
    }

    if (!findInMap(channel.list_user, client.socket_fd)){
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
        return;// ERR_NOSUCHNICK (401)
    }
    if (findInMap(channel.list_user, invited->socket_fd)){
        return; // ERR_USERONCHANNEL (443)
    }

    
    // Continue with normal invite logic using 'invited'
    
    return;
}

/*
The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>.

The target channel SHOULD exist (at least one user is on it). Otherwise, the server SHOULD reject the command with the ERR_NOSUCHCHANNEL numeric.

Only members of the channel are allowed to invite other users. Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.

Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator.

If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.

When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.


Numeric Replies:
*/
// RPL_INVITING (341)

// ERR_CHANOPRIVSNEEDED (482)