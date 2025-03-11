
#include "includes.hpp"
#include "Server.hpp"

void Server::pass(Message message, Client &client){
    std::cout << "PASS command recieved" << std::endl;
    if (message.params.size() < 1){
        ERR_NEEDMOREPARAMS(client);
        return;
    }
    if (client.state == REGISTERED){
        ERR_ALREADYREGISTERED(client);
        return;
    }
    if (message.params[0] != _password){
        client.state = ANONYMOUS;
        ERR_PASSWDMISMATCH(client);
        return;
    }
    client.state = ALLOWED;
    return;
}


// ERR_NEEDMOREPARAMS (461)
// ERR_ALREADYREGISTERED (462) 
// ERR_PASSWDMISMATCH (464) 
