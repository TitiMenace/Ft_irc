
#include "includes.hpp"
#include "Server.hpp"

void Server::pass(Message message, Client &client){
    std::cout << "PASS command recieved" << std::endl;
    if (message.params.size() < 1){
        dprintf(client.socket_fd, "461 ERR_NEEDMOREPARAMS\r\n");
        return;
    }
    if (client.state == ALLOWED || client.state == REGISTERED){
        dprintf(client.socket_fd, "462 ERR_ALREADYREGISTERED\r\n");
        return;
    }
    if (message.params[0] != _password){
        dprintf(client.socket_fd, "464 ERR_PASSWDMISMATCH\r\n");
        return;
    }
    client.state = ALLOWED;
    return;
}


// ERR_NEEDMOREPARAMS (461)
// ERR_ALREADYREGISTERED (462) 
// ERR_PASSWDMISMATCH (464) 
