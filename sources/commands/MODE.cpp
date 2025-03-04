#include "Server.hpp"
#include "includes.hpp"

void    Server::flagsGestion(Message message, Client &client){

    std::string flags;
    std::string channel = message.params[0];
   \
    std::cout << "on rentre dans la gestion des flags" << std::endl;
    if(message.params[1][0] == '+'){
        flags = message.params[1].substr(1, message.params.size());
        for (size_t i = 0; i < flags.size(); i++){
        
            switch (flags[i]){

                case 't':
                        if (!(_channel_list[channel].mode & TOPIC_ONLY_OP))
                            _channel_list[channel].mode += TOPIC_ONLY_OP;   
                case 'k':
                        if (message.params.size() >= 3){
                           std::cout << "le mot de passe " << message.params[2] << " est bien assigne" << std::endl;
                            _channel_list[channel].key = message.params[2];
                        }

                case 'i':
                        if (!(_channel_list[channel].mode & INVITE_ONLY)){
                            std::cout  << "channel passe bien en invite only" << std::endl;
                            _channel_list[channel].mode += INVITE_ONLY;
                        }
                
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

void Server::mode(Message message, Client &client) {
    
    std::cout << "mod appele avec " << message.params.size() << " arguments" << std::endl;

    if (message.params.empty())
        std::cout << "pas de arguments de mod" << std::endl;

    std::string channel = message.params[0];
    std::string arg;

    for ( std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++){
        if (it->second.nickname == channel){
            ERR_UMODEUNKNOWNFLAG(client, '0');
            return;
        }

    }
    std::cout << "fonctione mode pour le channel activated" << std::endl;    
    for ( std::map<std::string, Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++){
        if (it->first == channel){
        
            if (message.params.size() <= 1){
                ERR_UMODEUNKNOWNFLAG(client, '0');
                return;
            }
            else
                flagsGestion(message, client);
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