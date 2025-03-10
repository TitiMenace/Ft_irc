#include "Server.hpp"
#include "includes.hpp"
#include "Channel.hpp"




void    RPL_INVEXLIST(Client &client, Channel &channel){


    std::stringstream	soutput;

    std::string cname = channel.name;
    std::string output;

	soutput << "346 " << client.nickname << " ";
	soutput << channel.name << " ";
    soutput << "INVITE_ONLY";
    dprintf(2, "%s\n", output.c_str());
    dprintf(client.socket_fd, "%s\r\n", output.c_str());
	return;
}

void    RPL_ENDOFINVEXLIST(Client &client, Channel &channel){


    std::stringstream	soutput;

    std::string cname = channel.name;
    std::string output;

	soutput << "347 " << client.nickname << " ";
	soutput << channel.name << " :";
    soutput << "End of Channel Invite Exception List";
    dprintf(2, "%s\n", output.c_str());
    dprintf(client.socket_fd, "%s\r\n", output.c_str());
	return;
}

void    Server::channelflagsGestion(Message message, Client &client){

    std::string flags;
    std::string channel = message.params[0];
    size_t      nb_arg = 0;
   \
   if (message.params.size() <= 1){
        ERR_UMODEUNKNOWNFLAG(client, '0');
        return;
   }
   
   std::cout << "on rentre dans la gestion des flags" << std::endl;
    if(message.params[1][0] == '+'){
        flags = message.params[1].substr(1, message.params.size());
        for (size_t i = 0; i < flags.size(); i++){
        
            switch (flags[i]){

                case 't':
                            if (!(_channel_list[channel].mode & TOPIC_ONLY_OP))
                                _channel_list[channel].mode += TOPIC_ONLY_OP;  
                            break;
                     
                case 'k':
                            if (message.params.size() >= 3 + nb_arg){
                            std::cout << "le mot de passe " << message.params[nb_arg + 2] << " est bien assigne" << std::endl;
                            _channel_list[channel].key = message.params[nb_arg + 2];
                            nb_arg++;
                            }
                            break;

                case 'i':
                            if (!(_channel_list[channel].mode & INVITE_ONLY)){
                                std::cout  << "channel passe bien en invite only" << std::endl;
                                _channel_list[channel].mode += INVITE_ONLY;
                            }
                            break;

                case 'l':
                            if (message.params.size() >= 3 + nb_arg){
                                std::cout << "la size max" << message.params[nb_arg + 2] << " est bien assigne" << std::endl;
                                _channel_list[channel].key = message.params[nb_arg + 2];
                                nb_arg++;

                            }
                            break;
                
                case 'o':
                            if (message.params.size() >= 3 + nb_arg){
                                for(std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++){
                                    if (it->second.nickname == message.params[nb_arg + 2]){
                                    
                                        std::cout << "le user " << message.params[nb_arg + 2] << " est bien devenu operateur" << std::endl;
                                        _channel_list[channel].list_operator[it->first] = it->second;
                                        nb_arg++;
                                        std::cout << "le channel name = " << channel << std::endl;
                                        std::cout << "info user le fd : " << it->first << " puis le client nickname : " << it->second.nickname << std::endl;
                                        std::cout << "le nom de kaaris qui est stocke c'est : " << _channel_list[channel].list_operator[5].nickname << std::endl;

                                    }
                                }
                            }
                            break;

                default : 
                            ERR_UMODEUNKNOWNFLAG(client, '0');
                
            }
        }
    }
    else if(message.params[1][0] == '-'){
        flags = message.params[1].substr(1, message.params.size());
        for (size_t i = 0; i < flags.size(); i++){

            switch (flags[i]){

                case 't':
                            if (_channel_list[channel].mode & TOPIC_ONLY_OP)
                                _channel_list[channel].mode -= TOPIC_ONLY_OP;
                            break;   
                case 'k':
                            std::cout << "le mot de passe est bien enleve" << std::endl;
                            _channel_list[channel].key = "";
                            break;

                case 'i':
                            if (_channel_list[channel].mode & INVITE_ONLY){
                                std::cout  << "channel n'est plus en invite only" << std::endl;
                                _channel_list[channel].mode -= INVITE_ONLY;
                            }
                            break;
                case 'l':
                            std::cout << "size limit du channel est bien revenue par defaut" << std::endl;
                            _channel_list[channel].size_limit = SIZE_LIMIT;
                            break;
                
                case 'o':
                            for (std::map<int, Client>::iterator it = _channel_list[channel].list_operator.begin(); it != _channel_list[channel].list_operator.end(); it++){
                                if (it->second.nickname == message.params[2 + nb_arg]){
                                    std::cout << "on enleve le statut d'operateur au client : " << message.params[2 + nb_arg] << std::endl;
                                    _channel_list[channel].list_operator.erase(it);
                                }
                                else if (it == _channel_list[channel].list_operator.end())
                                    std::cout << "Can't find operator in the channel's list" << std::endl;
                            }
                            break;

                default :
                            ERR_UMODEUNKNOWNFLAG(client, flags[i]);
                
            }
        }
    }
    else
        ERR_UMODEUNKNOWNFLAG(client, '0');
        

}

void	RPL_CHANNELMODEIS(Channel &channel, Client &client){

    std::string modes = getchannelmodes(channel);
    std::string modargs = getchannelmodarg(channel);
    dprintf(client.socket_fd, "324 %s %s :%s %s...\r\n",client.nickname.c_str(), channel.name.c_str(), modes.c_str(), modargs.c_str());
}

void Server::mode(Message message, Client &client) {
    std::cout << "mode appele avec " << message.params.size() << " arguments" << std::endl;

    if (client.state != REGISTERED) {
        ERR_NOTREGISTERED(client);
        return;
    }

    if (message.params.empty()){
        std::cout << "pas de arguments de mod" << std::endl;
        return;
    }

    std::string target = message.params[0];
    std::string arg;

    for ( std::map<std::string, Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++){
        if (it->first == target){
            if (_channel_list[target].list_operator.find(client.socket_fd) == _channel_list[target].list_operator.end())
                ERR_CHANOPRIVSNEEDED(client, target);
            else if (message.params[1].empty())
                RPL_CHANNELMODEIS(_channel_list[target], client);
            else
                channelflagsGestion(message, client);
        }
        else
            ERR_NOSUCHCHANNEL(client, target);
    }
}

// · i: Set/remove Invite-only channel

// · t: Set/remove the restrictions of the TOPIC command to channel operators

// · k: Set/remove the channel key (password)

// · o: Give/take channel operator privilege

// · l: Set/remove the user limit to channel