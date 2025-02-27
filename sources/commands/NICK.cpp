#include "includes.hpp"
#include "Server.hpp"
#include "parsingUtils.hpp"
//special    =  %x5B-60 / %x7B-7D
//           ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
//bool charInSet(char c, std::string set) 
//nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
bool validateNickname(const std::string& nickname) {
    if (nickname.length() > 9 || nickname.length() < 1)
        return false;

    if (!isalpha(nickname[0]) && !charInSet(nickname[0], std::string("[]{}|\\`_^", 8)))
        return false;

    for (size_t i = 1; i < nickname.length(); ++i) {
        if (!isalpha(nickname[i]) && !isdigit(nickname[i]) && !charInSet(nickname[i], std::string("[]{}|\\`_^-", 9))) {
            return false;
        }
    }
    return true;
}

void Server::nick(Message message, Client &client){
    std::cout << "Command NICK starting" << std::endl;
    int argc = message.params.size();
    if (argc < 1){
        dprintf(client.socket_fd, "431 ERR_NONICKNAMEGIVEN\r\n");
        return;
    }
    if (!validateNickname(message.params[0])){
        dprintf(client.socket_fd, "432 ERR_ERRONEUSNICKNAME\r\n");  
        std::cerr << "Nickname send by client is erroneous." << std::endl;
        return;
    }
    client.nickname = message.params[0]; //actually need to check the thing

	std::cout << "====NICK COMMAND DONE======\n\n" << std::endl;
	std::cout << "For Client (" << client.socket_fd << ")\n\n";
	std::cout << "Nickname : "<< client.nickname << std::endl;
    return;
}

// ERR_ERRONEUSNICKNAME (432) invalid char used or something, and maybe size?
// ERR_NICKNAMEINUSE (433) check the _users map to see if a user aready has this name

// ERR_NICKCOLLISION (436) not possible i think?
