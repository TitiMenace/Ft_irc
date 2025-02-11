#include "includes.hpp"

int main(int __attribute__((unused))argc, char **__attribute__((unused))argv)
{
	if (argc <= 2){
        std::cerr << "Not enough arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
		return 1;
	}

	else if (argc > 3){
         std::cerr << "Too many arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
		 return 1;
	}
	Server	IrcServ(atoi(argv[1]));
}
