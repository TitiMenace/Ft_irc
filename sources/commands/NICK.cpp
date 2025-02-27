#include "includes.hpp"
#include "Server.hpp"
#include <stdio.h>
// bool validateNickname(string nickname){

// }

void Server::nick(Message message, Client &client){
    std::cout << "Command NICK starting" << std::endl;
    int argc = message.params.size();     
    if (argc < 1){
        dprintf(client.socket_fd, "431 ERR_NONICKNAMEGIVEN\r\n"); //send ERR_NONICKNAMEGIVEN (431) 
    }
    if (argc >= 1)
        client.nickname = message.params[0]; //actually need to check the thing
    dprintf(client.socket_fd, "432 ERR_ERRONEUSNICKNAME\r\n"); //send ERR_NONICKNAMEGIVEN (431) 
    std::cout << "Nickname send by client is erroneous." << std::endl;
    return;
}

// ERR_ERRONEUSNICKNAME (432) invalid char used or something, and maybe size?
// ERR_NICKNAMEINUSE (433) check the _users map to see if a user aready has this name

// ERR_NICKCOLLISION (436) not possible i think?
