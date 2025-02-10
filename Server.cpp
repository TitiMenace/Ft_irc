#include "Server.hpp"

Server::Server(void){}

Server::~Server(void){}

Server::Server(unsigned int nb) : fds(new int[nb]){};

Server::Server(const Server& copy){

	*this = copy;
}

Server &Server::operator=(const Server &type){

	(void)type;
	return *this; 
}

void	Server::runServer(void){

	std::cout << "Server running..." << std::endl;
	while (true){

		sleep(3);
		std::cout << "Server running..." << std::endl;
	}

}

