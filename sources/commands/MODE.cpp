#include "Server.hpp"
#include "Channel.hpp"

void Server::mode(Message message, Client &) {
    std::string channel = message.params[0]; 
    std::string modes = message.params[1];

    for (size_t i = 0; i < modes.length(); ++i) {
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
    }
}

// · i: Set/remove Invite-only channel

// · t: Set/remove the restrictions of the TOPIC command to channel operators

// · k: Set/remove the channel key (password)

// · o: Give/take channel operator privilege