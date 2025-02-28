#include "Server.hpp"

void Server::join(Message message, Client &client){
    if (client.state != REGISTERED)
        return;
    std::cout << message.command <<" command recieved from " << client.nickname << std::endl;
    return;
}