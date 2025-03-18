
#include "includes.hpp"
#include "Server.hpp"
#include "parsingUtils.hpp"
#include <vector>

#include "RPL.hpp"

void Server::kick(Message message, Client &client){
    std::cout << "KICK command recieved" << std::endl;
    if (message.params.size() < 2){
        ERR_NEEDMOREPARAMS(client, message);
        return;
    }

    // Parameters: <channel> <user> *( "," <user> ) [<comment>]
    std::string channel_name = message.params[0];
    
    if (!findInMap(_channel_list, channel_name)){
        std::cout << "channel dont exist" << std::endl;
        ERR_NOSUCHCHANNEL(client, channel_name);
        return;
    }
    
    Channel &channel = _channel_list[channel_name];
    
    if (!findInMap(channel.list_user, client.socket_fd)){
        std::cout << "need to be in the channel to kick someone" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }
    
    if (!findInMap(channel.list_user, client.socket_fd)){
        std::cout << "need to be in the channel to kick someone" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    if (!findInMap(channel.list_operator, client.socket_fd)){
        std::cout << "Needs to be operator to kick someone from channel" << std::endl;
        ERR_CHANOPRIVSNEEDED(client, channel_name);
        return; //  ERR_CHANOPRIVSNEEDED (482) 
    }

    std::string nickname = message.params[1];

    Client* kicked = findUser(nickname);
	if (kicked == NULL) {
        std::cout << "user to kick doesnt exists" << std::endl;
        ERR_NOSUCHNICK(client, nickname);
        return;// ERR_NOSUCHNICK (401)
    }

    if (!findInMap(channel.list_user, kicked->socket_fd)){
        std::cout << "kicked person needs to be in channel" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    if (message.params.size() > 2){
        RPL_KICK(channel, *kicked, kicked->nickname, client.nickname, message.params[2]);
        RPL_KICK(channel, client, kicked->nickname, client.nickname, message.params[2]);
    }
    else{
        RPL_KICK(channel, *kicked, kicked->nickname, client.nickname);
        RPL_KICK(channel, client, kicked->nickname, client.nickname);
    }

    channel.list_user.erase(kicked->socket_fd);
    channel.list_operator.erase(kicked->socket_fd);
    if (channel.list_user.empty())
        _channel_list.erase(channel.name);
    //check if channel is empty; if it is remove it
    std::cout << "User " << nickname << " has been kicked from " << channel_name << std::endl;
    //remove the user from the channel (if its in it)
    //send the thing


// This message may be sent from a server to a client to notify the client
// that someone has been removed from a channel. In this case, 
// the message <source> will be the client who sent the kick, 
// and <channel> will be the channel which the 
// target client has been removed from.


// ERR_USERNOTINCHANNEL (441)
// ERR_NOTONCHANNEL (442) 
    return;
}

