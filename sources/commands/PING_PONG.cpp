
#include "includes.hpp"
#include "Server.hpp"

void Server::ping(Message message, Client &client){
    std::cout << "Command PING revieved" << std::endl;

    std::string token = "";
    if (message.params.size() >= 1)
    	token = token + message.params[0];
    std::string output = "PONG : " + token;
    dprintf(client.socket_fd,"%s\n", output.c_str());
    return;
}

