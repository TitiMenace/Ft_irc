#include "Server.hpp"
#include <cerrno>
#include <csignal>

#define EPOLL_MAX_EVENTS 32
#define BUFFER_SIZE 1024

Server::Server(void){}

Server::~Server(void){

	std::cout << "Server shutting down" << std::endl;
	close(_master_fd);
}

Server::Server(int port, std::string password) : _password(password){

	//on cree le socket du server
	_master_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (_master_fd == -1) 
		throw std::runtime_error(strerror(errno));
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
	if ((bind(_master_fd, (SA*)&_servaddr, sizeof(_servaddr))) != 0) {
		close(_master_fd);
		throw std::runtime_error(strerror(errno));
	}
	std::cout << "Socket successfully binded" << std::endl;
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
		if (client.nickname.empty() && client.hostname.empty() && client.username.empty() && client.servername.empty())
			std::cout << "Unregistered";
		std::cout << client.nickname << " "<< client.hostname << " "
					<< client.username << " " << client.servername << " " << std::endl;
	}
	std::cout << std::endl;
}

void debug_message(const Message &message) {
	std::cout << "<" << message.command << "> [";
	for (std::size_t i = 0; i < message.params.size(); i++) {
		std::cout << "\"" << message.params[i] << "\"";
		if (i < message.params.size() - 1)
			std::cout << ", ";
	}
	std::cout << "]" << std::endl;
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
	event.events = EPOLLIN | EPOLLOUT; // | EPOLLHUP | EPOLLRDHUP;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event);
	//Add a client to the client map users
	Client client = {client_socket, ANONYMOUS, "", "", "", "", "", "", ""};
	_users[client_socket] = client;
}

bool	Server::_runCommand(Client &client, std::string &buffer, std::size_t &buffer_pos) {
	typedef void (Server::*CommandFunction)(Message, Client &);
	typedef std::map<std::string, CommandFunction>::value_type Command;
	static const Command commandsArray[] = {
		Command("PASS", &Server::pass),
		Command("NICK", &Server::nick),
		Command("USER", &Server::user),
		Command("PING", &Server::ping),
		Command("JOIN", &Server::join),
		Command("MODE", &Server::mode),
		Command("KICK", &Server::kick),
		Command("TOPIC", &Server::topic),
		Command("INVITE", &Server::invite),
		Command("PRIVMSG", &Server::privmsg)
	};
	static const std::map<std::string, CommandFunction> commands(commandsArray, commandsArray + 10);

	std::size_t pos = buffer.find("\r\n", buffer_pos);
	if (pos == std::string::npos)
		return false;
	std::string message_str = buffer.substr(buffer_pos, pos - buffer_pos);
	buffer_pos = pos + 2;

	Message message;
	if (!parseMessage(message_str, message)) {
		std::cerr << "Error: syntax error in message" << std::endl;
		return false;
	}

	std::cout << "Command received: "<< commands.count(message.command) << " ";
	debug_message(message);

	if (commands.count(message.command)) {
		CommandFunction commandFunc = commands.at(message.command);
		(this->*(commandFunc))(message, client);

	}
	return true;
}

void	Server::_disconnectClient(Client &client) {
	// Erase client from the channels they're connected to
	std::map<std::string, Channel>::iterator it;
	
	for (it = _channel_list.begin(); it != _channel_list.end(); ) {
		Channel &channel = it->second;
		channel.list_user.erase(client.socket_fd);
		channel.list_operator.erase(client.socket_fd);
		channel.list_invite.erase(client.socket_fd);
		
		// Erase the channel if there are no users
		std::map<std::string, Channel>::iterator tmp = it;
		it++;
		if (channel.list_user.empty())
			_channel_list.erase(tmp);
	}

	// Erase client from the user list
	close(client.socket_fd);
	_users.erase(client.socket_fd);
}

void	Server::_sendMessages(Client &client) {
	if (client.outBuffer.empty())
		return;

	ssize_t nbBytes = send(client.socket_fd, &client.outBuffer[0], client.outBuffer.size(), 0);
	if (nbBytes == -1)
		return;
	
	client.outBuffer = client.outBuffer.substr(nbBytes);
}

void	Server::_readMessages(struct epoll_event event) {
	int client_socket = event.data.fd;
	if (event.events & EPOLLIN) {
		//on lit le message
		char buffer[BUFFER_SIZE] = {0};
		int bytes_received = read(client_socket, buffer, BUFFER_SIZE);
		std::cout << "Bytes received: " << bytes_received << std::endl;

		if (bytes_received <= 0) {
			std::cout << "Client ended the connection" << std::endl;
			_disconnectClient(_users[client_socket]);
			return;
		}
		std::cout << "[" << client_socket << "] Message recieved: " << std::endl << std::string(buffer, bytes_received) << std::endl;					
		
		std::string &client_buffer = _users[client_socket].inBuffer;
		std::size_t buffer_pos = 0;//client_buffer.length();
		client_buffer.append(buffer, bytes_received);
	
		while (_runCommand(_users[client_socket], client_buffer, buffer_pos))
			;
		//clean buffer
		client_buffer = client_buffer.substr(buffer_pos);
	}
	if (event.events & EPOLLERR) {
		std::cout << "Client encountered an error" << std::endl;
		_disconnectClient(_users[client_socket]);
	}
}

bool Server::_running = true;

void	Server::_stopServer(int signal) {
	(void) signal;
	_running = false;
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

	signal(SIGINT, _stopServer);

	while (_running) {
		int events_received = epoll_wait(epoll_fd, events, EPOLL_MAX_EVENTS, -1);
		for (int i = 0; i < events_received; i++) {
			if (events[i].data.fd == _master_fd) {
				_acceptClient(epoll_fd);
			} else {
				if (events[i].events & EPOLLOUT) {
					_sendMessages(_users[events[i].data.fd]);
				}
				_readMessages(events[i]);
			}

			if (events[i].events & EPOLLIN) {
				std::cout << "[" << events[i].data.fd << "] ";
				std::cout << "Events received: " << events_received << std::endl;
				debug_epoll_events(events[i].events);
				debug_users_map(_users);
			}	 
		}
	}

	// Close open connections
	std::map<int, Client>::iterator it;
	for (it = _users.begin(); it != _users.end(); it++) {
		close(it->first);
	}
	close(epoll_fd);
}


