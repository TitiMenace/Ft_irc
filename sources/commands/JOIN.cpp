#include "Server.hpp"
#include "Channel.hpp"
#include "parsingUtils.hpp"
#include <vector>
#include "RPL.hpp"

// chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
// chanstring =/ %x2D-39 / %x3B-FF
//                 ; any octet except NUL, BELL, CR, LF, " ", "," and ":"

void Server::join(Message message, Client &client){

    if (client.state != REGISTERED) {
        std::cerr << "NOT REGISTERED" << std::endl;
        ERR_NOTREGISTERED(client);
        return;
    }
    
    if (message.params.empty()){
        std::cout << "---empty param issue ----- \n";
        ERR_NEEDMOREPARAMS(client, "JOIN");
        return;//notenoughparams to send

    }
    std::vector<std::string> params_channels_list;
    if (!parseChannelList(message.params[0], params_channels_list)){
        // ERR_BADCHANMASK (476)ERR_BADCHANMASK
        ERR_BADCHANMASK(client, message.params[0]);
        std::cout << "---parsechannel issue ----- \n";
        return;
    }
    std::vector<std::string> keys_list;
    if (message.params.size() >= 2){
        std::cout << message.params[1];
        //parse password listchannel
        if (!parseKeyList(message.params[1], keys_list)){
            std::cout << "---parsekeys issue ----- \n";
            return;
        }
    }
    std::cout << message.command <<" command recieved from " << client.nickname << std::endl;
    
    std::cout << "Parsed channels:" << std::endl;
    for (std::vector<std::string>::iterator it = params_channels_list.begin(); it != params_channels_list.end(); ++it) {
        std::cout << *it << std::endl;
    }
    
    std::cout << "Parsed keys:" << std::endl;
    for (std::vector<std::string>::iterator it = keys_list.begin(); it != keys_list.end(); ++it) {
        std::cout << *it << std::endl;
    }
    //make this into a loop for each of the parsed channels
    for(std::size_t i = 0; i < params_channels_list.size(); i++)
    {
        std::cout << "number of parsed channels : " << params_channels_list.size() << std::endl;
        std::string channel_name = params_channels_list[i];
        std::map<std::string, Channel>::iterator it= _channel_list.find(channel_name);
        
        if (it == _channel_list.end()) {
            _channel_list[channel_name] = Channel(channel_name, "topic", "password", 1);
            _channel_list[channel_name].members.insert(client.socket_fd);
            _channel_list[channel_name].operators.insert(client.socket_fd);
            for (std::set<int>::iterator it = _channel_list[channel_name].members.begin(); it != _channel_list[channel_name].members.end(); it++)
		        RPL_JOIN(client, channel_name, _users[*it]);
            RPL_NAMREPLY(client, _channel_list[channel_name], _users);
            RPL_ENDOFNAMES(client, _channel_list[channel_name]);
            continue;
        }
        // /join #canel1,#canel2,#acan3
        else if (_channel_list[channel_name].mode & KEY_PROTECTED &&
            (message.params.size() < 2 || (i < keys_list.size() &&
            _channel_list[channel_name].key != keys_list[i]))){
            ERR_BADCHANNELKEY(client, _channel_list[channel_name]);
            continue;
        }
        else if (_channel_list[channel_name].mode & USER_LIMIT &&
            _channel_list[channel_name].members.size() >= _channel_list[channel_name].size_limit){

            ERR_CHANNELISFULL(client, _channel_list[channel_name]);
            continue;
        }
        else if (_channel_list[channel_name].mode & INVITE_ONLY &&
            !_channel_list[channel_name].invites.count(client.socket_fd)){
            ERR_INVITEONLYCHAN(client, _channel_list[channel_name]);
            std::cerr << channel_name << " is an invite only channel" << client.nickname << " can't join it" << std::endl; 
            continue;
        }
        it->second.members.insert(client.socket_fd);
		for (std::set<int>::iterator it = _channel_list[channel_name].members.begin(); it != _channel_list[channel_name].members.end(); it++)
		    RPL_JOIN(client, channel_name, _users[*it]);
		RPL_NAMREPLY(client, _channel_list[channel_name], _users);
		RPL_ENDOFNAMES(client, _channel_list[channel_name]);
    }
    std::cerr << std::endl;
	// if (_channel_list.find(message.params[0]) =! it.end)
    // Channel to_join = _channel_list[message.params[0]];


    return;
}
