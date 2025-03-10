#include "Server.hpp"
#include "Channel.hpp"
#include "parsingUtils.hpp"
#include <vector>
// chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
// chanstring =/ %x2D-39 / %x3B-FF
//                 ; any octet except NUL, BELL, CR, LF, " ", "," and ":"


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
    dprintf(2, "%s", output.c_str());
    dprintf(client.socket_fd, "%s", output.c_str());
	return;
}


void Server::join(Message message, Client &client){
    
    // std::cerr << "param 1 " << message.params[0] << std::endl;
    // std::cerr << "param 2 " << message.params[1] << std::endl;
    // std::cerr << "param 3 " << message.params[2] << std::endl;
    // std::cerr << "param 4 " << message.params[3] << std::endl;
   
    if (client.state != REGISTERED) {
        std::cerr << "NOT REGISTERED" << std::endl;
        ERR_NOTREGISTERED(client);
        return;
    }
    
    if (message.params.empty()){
        std::cout << "---empty param issue ----- \n";
        ERR_NEEDMOREPARAMS(client);
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
            _channel_list[channel_name].list_user[client.socket_fd] = client;
            _channel_list[channel_name].list_operator[client.socket_fd] = client;
            dprintf(2, "Channel : %s a bien ete cree et %s est bien user et %s est bien operateur\r\n", _channel_list[channel_name].name.c_str(), _channel_list[channel_name].list_user[client.socket_fd].nickname.c_str(), _channel_list[channel_name].list_operator[client.socket_fd].nickname.c_str());
            joinMessage(client, channel_name);
            RPLNAMREPLY(client, _channel_list[channel_name]);
            RPL_ENDOFNAMES(client, _channel_list[channel_name]);
            continue;
        }
        // /join #canel1,#canel2,#acan3
        else if (_channel_list[channel_name].mode & KEY_PROTECTED &&
            (message.params.size() < 2 || (i < keys_list.size() &&
            _channel_list[channel_name].key != keys_list[i]))){
            dprintf(2, "incorrect key for channel %s or no key gven\r\n", channel_name.c_str());
            //ERR_BADCHANNELKEY (475) 
            continue;
        }
        else if (_channel_list[channel_name].mode & USER_LIMIT &&
            _channel_list[channel_name].list_user.size() >= _channel_list[channel_name].size_limit){

            ERR_CHANNELISFULL(client, _channel_list[channel_name]);
            dprintf(2, "User limit for channel %s has been reached\r\n", channel_name.c_str());
            continue;
        }
        else if (_channel_list[channel_name].mode & INVITE_ONLY &&
            findInMap(_channel_list[channel_name].list_invite,client.socket_fd) == NULL){
            ERR_INVITEONLYCHAN(client, _channel_list[channel_name]);
            std::cerr << channel_name << " is an invite only channel" << client.nickname << " can't join it" << std::endl; 
            continue;
        }
            it->second.list_user[client.socket_fd] = client;
            dprintf(2, "%s : est bien arrive dans le channel : %s\r\n", client.nickname.c_str(), _channel_list[channel_name].name.c_str());
            joinMessage(client, channel_name);
            RPLNAMREPLY(client, _channel_list[channel_name]);
            RPL_ENDOFNAMES(client, _channel_list[channel_name]);
    }
    std::cerr << std::endl;
	// if (_channel_list.find(message.params[0]) =! it.end)
    // Channel to_join = _channel_list[message.params[0]];


    return;
}
