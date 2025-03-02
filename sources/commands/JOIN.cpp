#include "Server.hpp"
#include "Channel.hpp"
// chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
// chanstring =/ %x2D-39 / %x3B-FF
//                 ; any octet except NUL, BELL, CR, LF, " ", "," and ":"
bool parseChanString(std::string &buffer, std::size_t &buffer_pos) {
    std::size_t i = buffer_pos;
    while (i < buffer.length()) {
        unsigned char ch = buffer[i];
        // Invalid characters: NUL (0x00), BELL (0x07), CR (0x0D), LF (0x0A), " " (0x20), "," (0x2C), ":" (0x3A)
        if (ch == 0x00 || ch == 0x07 || ch == 0x0A || ch == 0x0D || ch == 0x20 || ch == 0x2C || ch == 0x3A) {
            break;
        }
        i++;
    }
    if (i == buffer_pos) {
        return false;  // No valid characters found
    }
    buffer_pos = i;
    return true;
}

bool parseChannelID(std::string &buffer, std::size_t &buffer_pos) {
    std::size_t i = buffer_pos;

    for (int count = 0; count < 5; count++) {
        if (i >= buffer.length() || !(std::isdigit(buffer[i]) || (buffer[i] >= 'A' && buffer[i] <= 'Z'))) {
            return false; 
        }
        i++;
    }
    buffer_pos = i;
    return true;
}

// channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//                 [ ":" chanstring ]
bool parseChannel(std::string &buffer) {
    std::size_t buffer_pos = 0;
    if (buffer.length() <= 2 || buffer_pos >= buffer.length()) {
        return false;
    }
    char firstChar = buffer[buffer_pos];
    std::cout << firstChar << std::endl;
    if (firstChar != '#' && firstChar != '+' && firstChar != '&' && firstChar != '!') {
        return false;
    }
    buffer_pos++;
    if (firstChar == '!') {
        if (!parseChannelID(buffer, buffer_pos)) {
            return false;
        }
    }
    if (!parseChanString(buffer, buffer_pos)) {
        return false;
    }
    if (buffer_pos < buffer.length() && buffer[buffer_pos] == ':') {
        buffer_pos++;
        if (!parseChanString(buffer, buffer_pos)) {
            return false;
        }
    }
    if (buffer_pos != buffer.length())
        return false;
    return true;
}

void    joinMessage(Client &client, std::string channel_name){

    std::string cname = channel_name;//.substr(1, channel_name.size());
    dprintf(client.socket_fd, ":%s!%s@%s JOIN %s\r\n",client.nickname.c_str(),client.username.c_str(),client.hostname.c_str(), cname.c_str());
    return;
}

void    RPL_ENDOFNAMES(Client &client, Channel &channel){

    dprintf(2, "------------------DEBUG-------------------\n----------------channelname : %s---------------\n", channel.name.c_str());
    std::stringstream	soutput;
    std::string cname = channel.name;//.substr(1, channel.name.size());
    std::string output;
	//soutput << ":localhost ";

	soutput << "366 " << client.nickname;
	//soutput << " 353";

	soutput << " " << cname <<  " :End of /NAMES list"; 
    output = soutput.str();
    output += "\r\n";
     dprintf(2, "%s", output.c_str());
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
} 

void    RPLNAMREPLY(Client &client, Channel &channel){


    std::stringstream	soutput;

    std::string cname = channel.name;//.substr(1, channel.name.size());
    std::string output;
	//soutput << ":localhost ";

	soutput << "353 " << client.nickname;
	//soutput << " 353";

	soutput << " = " << cname <<  " : ";
    for (std::map<int, Client>::iterator it = channel.list_user.begin(); it != channel.list_user.end(); it++){
        if (channel.list_operator.find(it->first) != channel.list_operator.end()){
            soutput << "@";
        }
	    soutput << it->second.nickname << " ";
    }
    output = soutput.str();
    output.erase(output.size() - 1);
    output += "\r\n";
    std::cout << "je suis un caca dani caca le client socket fd est" << client.socket_fd << std::endl;
    dprintf(2, "%s", output.c_str());
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}


void Server::join(Message message, Client &client){
    
    std::cerr << "param 1 " << message.params[0] << std::endl;
    std::cerr << "param 2 " << message.params[1] << std::endl;
    std::cerr << "param 3 " << message.params[2] << std::endl;
    std::cerr << "param 4 " << message.params[3] << std::endl;
   
    if (!(client.state & ALLOWED) && !(client.state & REGISTERED)) {
        std::cerr << "NOT ALLOWED OR NOT REGISTERED" << std::endl;
        return;
    }
    
    if (message.params.empty()){
        std::cout << "---empty param issue ----- \n";
        
        return;//notenoughparams to send

    }
    if (!parseChannel(message.params[0])){
        std::cout << "---parsechannel issue ----- \n";
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
        joinMessage(client, channel_name);
        RPLNAMREPLY(client, _channel_list[channel_name]);
        RPL_ENDOFNAMES(client, _channel_list[channel_name]);
        return;
    }

    else if (_channel_list[channel_name].list_user.size() >= _channel_list[channel_name].size_limit){

        ERR_CHANNELISFULL(client, _channel_list[channel_name]);
        dprintf(2, "User limit for channel %s has been reached\r\n", channel_name.c_str());
	    return;
    }
    else if (_channel_list[channel_name].mode & INVITE_ONLY){ //&& check_invite_list(client)){
        ERR_INVITEONLYCHAN(client, _channel_list[channel_name]);
        std::cerr << channel_name << " is an invite only channel" << client.nickname << " can't join it" << std::endl; 
        return; 
    }
	    it->second.list_user[client.socket_fd] = client;
	    dprintf(2, "%s : est bien arrive dans le channel : %s\r\n", client.nickname.c_str(), _channel_list[channel_name].name.c_str());
        joinMessage(client, channel_name);
        RPLNAMREPLY(client, _channel_list[channel_name]);
        RPL_ENDOFNAMES(client, _channel_list[channel_name]);

	// if (_channel_list.find(message.params[0]) =! it.end)
    // Channel to_join = _channel_list[message.params[0]];


    return;
}
