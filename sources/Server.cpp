#include "includes.hpp"
#define EPOLL_MAX_EVENTS 32
#define BUFFER_SIZE 2

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

void debug_users_map(std::map<int, Client> users) {
	std::cout << "List of users :" << std::endl;
	for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it) {
		Client& client = it->second;  // Get reference to Client object
		std::cout << "[" << client.socket_fd << "] ";
		if (client.nickname.empty() && client.hostname.empty() && client.username.empty())
			std::cout << "Unregistered";
		std::cout << client.nickname << " "
					<< client.hostname << " "
					<< client.username << std::endl;
	}
	std::cout << std::endl;
}


void	Server::_acceptClient(int epoll_fd) {
	struct epoll_event event;

	int client_socket = accept(_master_fd, (SA*)&_servaddr, (socklen_t *)&_addrlen);
	if (client_socket < 0){
		perror("accept");
		return;
	}
	std::cout << "New connection, fd: " << "[" << client_socket  << "]" << std::endl;

	event.data.fd = client_socket;
	event.events = EPOLLIN; // | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event);
	//Add a client to the client map users
	Client client = {client_socket, "", "", "", ""};
	_users[client_socket] = client;
}

bool	Server::_runCommand(std::string &buffer, std::size_t &buffer_pos) {
	std::size_t pos = buffer.find("\r\n", buffer_pos);
	if (pos == std::string::npos)
		return false;
	std::string message = buffer.substr(buffer_pos, pos - buffer_pos);
	buffer_pos = pos + 2;

	// this is equvalent to startswith, we check only the first index
	// see: https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a
	if (message.rfind("NICK ", 0) == 0) {
		std::cout << "Command NICK received" << std::endl;
	} else if (message.rfind("USER ", 0) == 0) {
		std::cout << "Command USER received" << std::endl;
	} else {
		pos = message.find(" ");
		if (pos != std::string::npos)
			message = message.substr(0, pos);
		std::cout << "ERROR: Unknown command: " << message << std::endl;
	}
	return true;
}



void	Server::_readMessages(struct epoll_event event) {
	int client_socket = event.data.fd;
	if (event.events & EPOLLIN) {
		//on lit le message
		char buffer[BUFFER_SIZE] = {0};
		int bytes_received = read(client_socket, buffer, BUFFER_SIZE);
		std::cout << "Bytes received: " << bytes_received << std::endl;

		if (bytes_received <= 0) {
			//remove the client from the map users
			std::cout << "Client ended the connection" << std::endl;
			_users.erase(client_socket);
			close(client_socket);
			return;
		}
		std::cout << "[" << client_socket << "] Message recieved: " << std::endl << std::string(buffer, bytes_received) << std::endl;					
		
		std::string &client_buffer = _users[client_socket].buffer;
		std::size_t buffer_pos = 0;//client_buffer.length();
		client_buffer.append(buffer, bytes_received);
	
		while (_runCommand(client_buffer, buffer_pos))
			;
		//clean buffer
		client_buffer.substr(buffer_pos);
	}
	if (event.events & EPOLLERR) {
		std::cout << "Client encountered an error" << std::endl;
		//remove client from user map if close
		_users.erase(client_socket);
		close(client_socket);
	}
}

void	Server::runServer(void){
	std::cout << "Server password is " << _password << std::endl;

	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	
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
			std::cout << "[" << events[i].data.fd << "] ";
			debug_epoll_events(events[i].events);
			if (events[i].data.fd == _master_fd) {
				_acceptClient(epoll_fd);
			} else {
				_readMessages(events[i]);
			}
			debug_users_map(_users); 
		}
	}
}


