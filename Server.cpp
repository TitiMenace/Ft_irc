#include "includes.hpp"
#define EPOLL_MAX_EVENTS 32

Server::Server(void){}

Server::~Server(void){

	std::cout << "Server shutting down" << std::endl;
	close(_master_fd);
}

Server::Server(int port){
	
	try {
		
		//on cree le socket du server
		_master_fd = socket(AF_INET, SOCK_STREAM, 0); 
		if (_master_fd == -1) 
			throw Aziz();
		else
			std::cout << "Socket successfully created..." << std::endl;

		std::memset(&_servaddr, 0, sizeof(_servaddr));
		
		//ici on configure la structur sockaddr qui contient les infos pour identifier
		//une adresse reseau commme une adresse IP et le port
		_servaddr.sin_family = AF_INET; //famille d'adresse ici IPv4
		_servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //adresse IP que le socket va use
		_servaddr.sin_port = htons(port); //Le port en format reseau
    		//ici on autorise le socket a reutiliser la meme adresse multiple fois
		int opt = 1;
		setsockopt(_master_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		
		_addrlen = sizeof(_servaddr);
		if ((bind(_master_fd, (SA*)&_servaddr, sizeof(_servaddr))) != 0)
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
	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	char buffer[1024] = {0};
	const char *response = "\nmoist\n";
	
	//le serv va se mettre a ecouter les connexions
	std::cout << "Server running" << std::endl;

	struct epoll_event event;
	event.data.fd = _master_fd;
	event.events = EPOLLIN;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _master_fd, &event);

	listen(_master_fd, 3);

	std::cout << "Waiting for connection..." << std::endl;
	//le serv "boucle" grace a la fonction bloquante accept tant que on a pas recu une demande de connexion

	struct epoll_event events[EPOLL_MAX_EVENTS];

	while (true) {
		int events_received = epoll_wait(epoll_fd, events, EPOLL_MAX_EVENTS, -1);

		for (int i = 0; i < events_received; i++) {
			if (events[i].data.fd == _master_fd) {
				int client_socket = accept(_master_fd, (SA*)&_servaddr, (socklen_t *)&_addrlen);
				if (client_socket < 0){
					perror("accept");
					return;
				}

				event.data.fd = client_socket;
				event.events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event);
			} else {
				int client_socket = events[i].data.fd;
				//on lit le message
				read(client_socket, buffer, 1024);
				std::cout << "Message recieved: " << buffer << std::endl;
				
				send(client_socket, response, strlen(response), 0);
				std::cout << "Reponse envoyee" <<  std::endl;
			}
		}
	}
}


