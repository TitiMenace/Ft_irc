#include "Server.hpp"
#include "Channel.hpp"
#include "parsingUtils.hpp"

#include "RPL.hpp"

void Server::invite(Message message, Client &client) {
std::cout << "invite command starting"<< std::endl;
    if (message.params.size() < 2){ 
        std::cout << "not enough params"<< std::endl;
        ERR_NEEDMOREPARAMS(client, message);
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

    if (!channel.members.count(client.socket_fd)){
        std::cout << "need to be in the channel to invite someone in it" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    if (!channel.operators.count(client.socket_fd) && channel.mode & INVITE_ONLY){
        std::cout << "Needs to be operator to invite in invite only" << std::endl;
        ERR_CHANOPRIVSNEEDED(client, channel_name);
        return; //  ERR_CHANOPRIVSNEEDED (482) 
    }


    Client* invited;
    if ((invited = findUser(nickname)) == NULL) {
        std::cout << "user to invite doesnt exists" << std::endl;
        ERR_NOSUCHNICK(client, nickname);
        return;// ERR_NOSUCHNICK (401)
    }
    if (invited->state != REGISTERED){
        ERR_NOSUCHNICK(client, nickname);
        //whatever error you knw
        return;
    }
    if (channel.members.count(invited->socket_fd)){
        std::cout << "user to invite already in the channel" << std::endl;
        ERR_USERONCHANNEL(client, channel_name, invited->nickname);
        return; // ERR_USERONCHANNEL (443)
    }

    // Continue with normal invite logic using 'invited'
    channel.invites.insert(invited->socket_fd);
    std::cout << "invite list:" << std::endl;
    for (std::set<int>::iterator it = channel.invites.begin(); it != channel.invites.end(); ++it) {
        std::cout << _users[*it].nickname << std::endl;
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
