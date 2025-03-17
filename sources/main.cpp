
#include "includes.hpp"
#include <stdint.h>
#include "Server.hpp"
#include "parsingUtils.hpp"

int main(int argc, char **argv)
{
	if (argc <= 2){
        std::cerr << "Error: Not enough arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
		return 1;
	}

	else if (argc > 3){
         std::cerr << "Error: Too many arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
		 return 1;
	}

	uint16_t port;
	if (!parseU16(argv[1], port)) {
		std::cerr << "Error: Invalid port number" << std::endl;
		return 1;
	}

	try {
		Server	ircServ(port, (std::string)argv[2]);

		ircServ.runServer();
	} catch (const std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
