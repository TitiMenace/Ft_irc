
#include "includes.hpp"
#include "Server.hpp"

void Server::ping(Message message, Client &client){
    std::string token = "";
    if (message.params.size() >= 1)
    	token = token + message.params[0];
    std::string output = "PONG :" + token;
    output += "\r\n";
    client.outBuffer += output;
    return;
}

