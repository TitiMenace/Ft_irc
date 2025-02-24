
#include "includes.hpp"

int main(int argc, char **argv)
{
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
