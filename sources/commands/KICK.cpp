
#include "includes.hpp"
#include "Server.hpp"
#include "parsingUtils.hpp"
#include <vector>
void	RPL_KICK(Channel &channel, Client &client, std::string kicked, std::string source, std::string comment = ""){
    if (!comment.empty())
        dprintf(client.socket_fd, ":%s KICK %s %s :%s\r\n",source.c_str(),channel.name.c_str(),kicked.c_str(),  comment.c_str());
    else
        dprintf(client.socket_fd, ":%s KICK %s %s\r\n",source.c_str(),channel.name.c_str(),kicked.c_str());
    
}
void Server::kick(Message message, Client &client){
    std::cout << "KICK command recieved" << std::endl;
    if (message.params.size() < 2){
        ERR_NEEDMOREPARAMS(client);
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

    if (!findInMap(channel.list_operator, client.socket_fd)){
        std::cout << "Needs to be operator to kick someone from channel" << std::endl;
        ERR_CHANOPRIVSNEEDED(client, channel_name);
        return; //  ERR_CHANOPRIVSNEEDED (482) 
    }

    if (!findInMap(channel.list_user, client.socket_fd)){
        std::cout << "need to be in the channel to kick someone" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    std::string nickname = message.params[1];

    Client* kicked = NULL;
    
    for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (it->second.nickname == nickname) {
            kicked = &(it->second);
            break;
        }
    }

    if (kicked == NULL) {
        std::cout << "user to kick doesnt exists" << std::endl;
        ERR_NOSUCHNICK(client, channel_name);
        return;// ERR_NOSUCHNICK (401)
    }

    if (!findInMap(channel.list_user, kicked->socket_fd)){
        std::cout << "kicked person needs to be in channel" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    if (message.params.size() > 2){
        RPL_KICK(channel, *kicked, kicked->nickname, client.nickname, message.params[3]);
        RPL_KICK(channel, client, kicked->nickname, client.nickname, message.params[3]);
    
    }
    else{
        RPL_KICK(channel, *kicked, kicked->nickname, client.nickname);
        RPL_KICK(channel, client, kicked->nickname, client.nickname);
    }

    channel.list_user.erase(kicked->socket_fd);
    channel.list_operator.erase(kicked->socket_fd);
    
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

