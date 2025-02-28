#include "Server.hpp"

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
    
    if (client.state != REGISTERED)
        return;//not registered error to send
        std::cout << message.command <<" command recieved from " << client.nickname << std::endl;
    if (message.params.empty())
		return;//notenoughparams to send
    if (!parseChannel(message.params[0]))
        return;
    std::string channel_name = message.params[0] 
    
    // if (_channel_list.find(message.params[0]) =! it.end)
    // Channel to_join = _channel_list[message.params[0]];


    return;
}