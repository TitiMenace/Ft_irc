#include "includes.hpp"
#include "Server.hpp"
#include "parsingUtils.hpp"
#include "RPL.hpp"

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
   
    if (client.state != ALLOWED && client.state != REGISTERED) {
        std::cerr << "NOT ALLOWED OR NOT REGISTERED" << std::endl;
        return;
    }

    if (message.params.size() < 1){
        ERR_NONICKNAMEGIVEN(client);
        return;
    }

	std::string nickname = message.params[0];
	if (client.nickname == nickname)
		return;

    if (!validateNickname(nickname)){
        ERR_ERRONEUSNICKNAME(client, nickname);
        return;
    }
    
	if (findUser(nickname)) {
		ERR_NICKNAMEINUSE(client,  nickname);
		return;
	}

	client.nickname = nickname;
    if (!client.username.empty() && client.state == ALLOWED){
        client.state = REGISTERED;//send RPL_WELCOME and stuff
		RPL_WELCOME(client);
        RPL_ISSUPPORT(client);
    }
    return;
}

// ERR_ERRONEUSNICKNAME (432) invalid char used or something, and maybe size?
// ERR_NICKNAMEINUSE (433) check the _users map to see if a user aready has this name

// ERR_NICKCOLLISION (436) not possible i think?
