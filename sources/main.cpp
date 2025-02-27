
#include "includes.hpp"
#include <stdlib.h> //fuckyou atoi
#include "Server.hpp"
int main(int argc, char **argv)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("Hostname: %s\n", hostname);
    } else {
        perror("gethostname");
    }
	if (argc <= 2){
        std::cerr << "Not enough arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
		return 1;
	}

	else if (argc > 3){
         std::cerr << "Too many arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
		 return 1;
	}

	Server	IrcServ(atoi(argv[1]), (std::string)argv[2]);

	IrcServ.runServer();
}
