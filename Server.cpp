#include "includes.hpp"
#define EPOLL_MAX_EVENTS 32

Server::Server(void){}

Server::~Server(void){

	std::cout << "Server shutting down" << std::endl;
	close(_master_fd);
}

Server::Server(int port, std::string password) : _password(password){
	
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

void debug_epoll_events(int event) {
	if (event & EPOLLIN)
		std::cout << "EPOLLIN ";
	if (event & EPOLLOUT)
		std::cout << "EPOLLOUT ";
	if (event & EPOLLERR)
		std::cout << "EPOLLERR ";
	if (event & EPOLLHUP)
		std::cout << "EPOLLHUP ";
	if (event & EPOLLRDHUP)
		std::cout << "EPOLLRDHUP ";
	std::cout << std::endl;
}

void	Server::runServer(void){
	std::cout << "Server password is " << _password << std::endl;

	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	char buffer[1024] = {0};
	
	//le serv va se mettre a ecouter les connexions
	std::cout << "Server running" << std::endl;

	struct epoll_event event;
	event.data.fd = _master_fd;
	event.events = EPOLLIN; // | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _master_fd, &event);

	listen(_master_fd, 3);

	std::cout << "Waiting for connection..." << std::endl;
	//le serv "boucle" grace a la fonction bloquante accept tant que on a pas recu une demande de connexion

	struct epoll_event events[EPOLL_MAX_EVENTS];

	while (true) {
		int events_received = epoll_wait(epoll_fd, events, EPOLL_MAX_EVENTS, -1);
		std::cout << "Events received: " << events_received << std::endl;
		for (int i = 0; i < events_received; i++) {
			if (events[i].data.fd == _master_fd) {
				int client_socket = accept(_master_fd, (SA*)&_servaddr, (socklen_t *)&_addrlen);
				if (client_socket < 0){
					perror("accept");
					return;
				}
				std::cout << "New connection, fd: " << client_socket << std::endl;

				event.data.fd = client_socket;
				event.events = EPOLLIN; // | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event);
			} else {
				std::cout << "[" << events[i].data.fd << "] ";
				debug_epoll_events(events[i].events);
				if (events[i].events & EPOLLIN) {
					int client_socket = events[i].data.fd;
					//on lit le message
					int bytes_received = read(client_socket, buffer, 1024);
					std::cout << "Bytes received: " << bytes_received << std::endl;

					if (bytes_received <= 0) {
						std::cout << "Client ended the connection" << std::endl;
						close(client_socket);
						continue;
					}
					std::cout << "[" << client_socket << "] Message recieved: " << std::endl << std::string(buffer, bytes_received) << std::endl;					
					send(client_socket, buffer, bytes_received, 0);
					std::cout << "Response sent" <<  std::endl;
				}
				if (events[i].events & EPOLLERR) {
					std::cout << "Client encountered an error" << std::endl;
					close(events[i].data.fd);
				}
			}
		}
	}
}


