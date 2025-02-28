#include "Server.hpp"
#include "Channel.hpp"

// channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//                 [ ":" chanstring ]
bool parseChannel(std::string){
    return true;
}
// chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
// chanstring =/ %x2D-39 / %x3B-FF
//                 ; any octet except NUL, BELL, CR, LF, " ", "," and ":"
bool parseChanString(std::string){
    return true;
}
// channelid  = 5( %x41-5A / digit )   ; 5( A-Z / 0-9 )
bool parseChannelID(std::string){
    return true;
}

void Server::join(Message message, Client &client){
    
    if (client.state != REGISTERED){
        return;//not registered error to send
    }
    
    if (message.params.empty()){
        return;//notenoughparams to send
    }
    if (!parseChannel(message.params[0])){
        return;
    }
    std::cout << message.command <<" command recieved from " << client.nickname << std::endl;
    

    std::string channel_name = message.params[0];
    std::map<std::string, Channel>::iterator it= _channel_list.find(channel_name);
    
    if (it == _channel_list.end()) {
    	

	_channel_list[channel_name] = Channel(channel_name, "topic", "password", 1);
	_channel_list[channel_name].list_user[client.socket_fd] = client;
	_channel_list[channel_name].list_operator[client.socket_fd] = client;
	dprintf(2, "Channel : %s a bien ete cree et %s est bien user et %s est bien operateur\r\n", _channel_list[channel_name].name.c_str(), _channel_list[channel_name].list_user[client.socket_fd].nickname.c_str(), _channel_list[channel_name].list_operator[client.socket_fd].nickname.c_str());
	return;
    }

    else if (_channel_list[channel_name].list_user.size() >= _channel_list[channel_name].size_limit){
    		dprintf(2, "User limit for channel %s has been reached\r\n", channel_name.c_str());
	    return;
    }
	
	    it->second.list_user[client.socket_fd] = client;
	    dprintf(2, "%s : est bien arrive dans le channel : %s\r\n", client.nickname.c_str(), _channel_list[channel_name].name.c_str());
	// if (_channel_list.find(message.params[0]) =! it.end)
    // Channel to_join = _channel_list[message.params[0]];


    return;
}
