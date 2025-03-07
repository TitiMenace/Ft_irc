#include "Server.hpp"
#include "includes.hpp"
#include "Channel.hpp"

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
                case 'k':
                        if (message.params.size() >= 3 + nb_arg){
                           std::cout << "le mot de passe " << message.params[nb_arg + 2] << " est bien assigne" << std::endl;
                           _channel_list[channel].key = message.params[nb_arg + 2];
                           nb_arg++;
                        }

                case 'I':
                        if (!(_channel_list[channel].mode & INVITE_ONLY)){
                            std::cout  << "channel passe bien en invite only" << std::endl;
                            _channel_list[channel].mode += INVITE_ONLY;
                        }
                case 'l':
                        if (message.params.size() >= 3 + nb_arg){
                            std::cout << "la size max" << message.params[nb_arg + 2] << " est bien assigne" << std::endl;
                            _channel_list[channel].key = message.params[nb_arg + 2];
                            nb_arg++;

                        }
                
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
                case 'k':
                           std::cout << "le mot de passe est bien enleve" << std::endl;
                           _channel_list[channel].key = "";

                case 'I':
                        if (_channel_list[channel].mode & INVITE_ONLY){
                            std::cout  << "channel n'est plus en invite only" << std::endl;
                            _channel_list[channel].mode -= INVITE_ONLY;
                        }
                case 'l':
                            std::cout << "size limit du channel est bien revenue par defaut" << std::endl;
                            _channel_list[channel].size_limit = SIZE_LIMIT;
                
                case 'o':
                            ERR_UMODEUNKNOWNFLAG(client, flags[i]);
                default :
                            ERR_UMODEUNKNOWNFLAG(client, flags[i]);
                
            }
        }
    }
    else
        ERR_UMODEUNKNOWNFLAG(client, '0');
        

}

void    Server::userflagsGestion(Message message, Client client){

    std::string user = message.params[0];
    std::string flags;
   // size_t  nb_args = 0;

    if (message.params.size() <= 1){
        std::cout << "missing flags" << std::endl;
        ERR_UMODEUNKNOWNFLAG(client, '0');
        return;
   }
  /* if(message.params[1][0] == '+'){
        flags = message.params[1].substr(1, message.params.size());
        for (size_t i = 0; i < flags.size(); i++){
        
            switch (flags[i]){

                case 't':
                            break;
                default:
                            break;
            }
        }  
    }*/
    ERR_UMODEUNKNOWNFLAG(client, '0');
    return;

}
 
void	RPL_CHANNELMODEIS(Channel &channel, Client &client){

    std::string modes = getchannelmodes(channel);
    std::string modargs = getchannelmodarg(channel);
    dprintf(client.socket_fd, "324 %s %s :%s %s...\r\n",client.nickname.c_str(), channel.name.c_str(), modes.c_str(), modargs.c_str());
}

void Server::mode(Message message, Client &client) {
    
    std::cout << "mode appele avec " << message.params.size() << " arguments" << std::endl;

    if (message.params.empty()){
        std::cout << "pas de arguments de mod" << std::endl;
        return;
    }

    std::string target = message.params[0];
    std::string arg;

    if (target[0] != '#'){
        for ( std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++){
            
            if (it == _users.end())
                ERR_NOSUCHNICK(client, "");
            else if (it->second.nickname != client.nickname)
                ERR_USERSDONTMATCH(client);
            else
                userflagsGestion(message, client);
        }
    }
    else {
        std::cout << "fonctione mode pour le channel activated" << std::endl;    
        for ( std::map<std::string, Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++){
            if (it->first == target){
            
                if (_channel_list[target].list_operator.find(client.socket_fd) != _channel_list[target].list_operator.end())
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
    return;
}
    //if (!message.params[2].empty())
      //  arg = message.params[2];

    /*for (size_t i = 0; i < modes.lengthRR_UMODEUNKNOWNFLAG(Client (); ++i) {
        char mode = modes[i];
        if (mode == '+' || mode == '-') {
            std::cout << channel << " has mode " << mode << std::endl;
        }
        else{
            std::cout << "incorrect begin char for mode : " << mode << std::endl;    
        }
    }
    for (size_t i = 2; i < message.params.size(); ++i) {
        std::cout << "Mode parameter: " << message.params[i] << std::endl;
    }}

// · i: Set/remove Invite-only channel

// · t: Set/remove the restrictions of the TOPIC command to channel operators

// · k: Set/remove the channel key (password)

// · o: Give/take channel operator privilege
*/