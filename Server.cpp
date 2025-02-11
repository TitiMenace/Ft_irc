#include "Server.hpp"

Server::Server(void){}

Server::~Server(void){}

Server::Server(int port){
	
	try {
		sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
		if (sock_fd == -1) 
			throw Aziz();
		else
			std::cout << "Socket successfully created..." << std::endl;

		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
		servaddr.sin_port = htons(port);

		if ((bind(sock_fd, (SA*)&servaddr, sizeof(servaddr))) != 0)
			throw Aziz();
		else
			std::cout << "Socket successfully binded" << std::endl;
	} catch  (std::exception &e){
		std::cerr << "TCP connect failed..." << std::endl;	
	}
}

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



