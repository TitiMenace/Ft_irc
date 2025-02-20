#include "Server.hpp"

Server::Server(void){}

Server::~Server(void){

	std::cout << "Server shutting down" << std::endl;
	close(socket_in);
	close(sock_fd);
}

Server::Server(int port){
	
	try {
		
		//on cree le socket du server
		sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
		if (sock_fd == -1) 
			throw Aziz();
		else
			std::cout << "Socket successfully created..." << std::endl;

		std::memset(&servaddr, 0, sizeof(servaddr));
		
		//ici on configure la structur sockaddr qui contient les infos pour identifier
		//une adresse reseau commme une adresse IP et le port
		servaddr.sin_family = AF_INET; //famille d'adresse ici IPv4
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //adresse IP que le socket va use
		servaddr.sin_port = htons(port); //Le port en format reseau

		addrlen = sizeof(servaddr);
		socket_in = -1;
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

	char buffer[1024] = {0};
	const char *response = "\nmoist\n";

	//le serv va se mettre a ecouter les connexions
	std::cout << "Server running" << std::endl;
	listen(sock_fd, 3);

	std::cout << "Waiting for connection..." << std::endl;
	//le serv "boucle" grace a la fonction bloquante accept tant que on a pas recu une demande de connexion

	socket_in = accept(sock_fd, (SA*)&servaddr, (socklen_t *)&addrlen);
	if (socket_in < 0){
		perror("accept");
		return;
	}

	//on lit le message
	read(socket_in, buffer, 1024);
	std::cout << "Message recieved: " << buffer << std::endl;

	send(socket_in, response, strlen(response), 0);
	std::cout << "Reponse envoyee" <<  std::endl;

}

void	Server::inputLoop(void){


}



