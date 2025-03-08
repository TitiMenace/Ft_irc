
#include "includes.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "parsingUtils.hpp"

void	sendChannelTopic(Channel &channel, std::string topic, Client &client){
    
	std::cout << "on envoit le topic a tout le monde dans le channel" << std::endl;
    
    for (std::map<int, Client>::iterator it = channel.list_user.begin(); it != channel.list_user.end(); it++){
        dprintf(it->first, ":%s TOPIC %s :%s\r\n",client.nickname.c_str(), channel.name.c_str(), topic.c_str());
	
        //If RPL_TOPIC is returned to the client sending this command,
        //  RPL_TOPICWHOTIME SHOULD also be sent to that client.
    }
	
}
void	RPL_TOPIC(Channel &channel, Client &client){
    dprintf(client.socket_fd, "332 %s %s :%s\r\n",client.nickname.c_str(), channel.name.c_str(), channel.topic.c_str());
}
void	RPL_NOTOPIC(Channel &channel, Client &client){
    dprintf(client.socket_fd, "331 %s %s :%s\r\n",client.nickname.c_str(), channel.name.c_str(), "No topic is set");
}

void Server::topic(Message message, Client &client){
    std::cout << "TOPIC command recieved" << std::endl;
    if (message.params.size() < 1){ 
        std::cout << "not enough params"<< std::endl;
        ERR_NEEDMOREPARAMS(client);
        return; // ERR_NEEDMOREPARAMS (461)
    }
    //checking channel part
    std::string channel_name = message.params[0];
    
    if (!findInMap(_channel_list, channel_name)){
        std::cout << "channel dont exist" << std::endl;
        ERR_NOSUCHCHANNEL(client, channel_name);
        return;
    }
    
    Channel &channel = _channel_list[channel_name];
    std::cout << "channel "<< channel.name << "exists" << std::endl;
    
    if (message.params.size() < 2){ 
        std::cout << "printing channel topic"<< std::endl;
        if (channel.topic.empty())
            RPL_NOTOPIC(channel,client);
        else
            RPL_TOPIC(channel,client);    
        // do actually print the topic stuff
        return;
    }
    
    //changing topic part
    std::string topic = message.params[1];
    if (!findInMap(channel.list_user, client.socket_fd)){
        std::cout << "need to be in the channel to change the topic" << std::endl;
        ERR_NOTONCHANNEL(client, channel_name);
        return; // ERR_NOTONCHANNEL (442)
    }

    if (!findInMap(channel.list_operator, client.socket_fd) && channel.mode & TOPIC_ONLY_OP){
        std::cout << "Needs to be operator to change topic in topic protected mode" << std::endl;
        ERR_CHANOPRIVSNEEDED(client, channel_name);
        return; //  ERR_CHANOPRIVSNEEDED (482) 
    }

    channel.topic = topic;
    sendChannelTopic(channel,topic,client);


    return;
}


// ERR_NEEDMOREPARAMS (461)
// ERR_ALREADYREGISTERED (462) 
// ERR_PASSWDMISMATCH (464) 