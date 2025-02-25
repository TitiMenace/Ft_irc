
#include "includes.hpp"

Client::Client(void){}

Client::~Client(void){

	std::cout << "Client shutting down" << std::endl;
}

Client::Client(int fd) : _nickname(""), _hostname(""), _username(""), _socket_fd(fd){
	std::cout << "Client registered" << std::endl;
}

Client::Client(const Client& copy){

	*this = copy;
}

Client &Client::operator=(const Client &type){

	_nickname = type._nickname;
	_hostname = type._hostname;
	_username = type._username;
	_socket_fd = type._socket_fd;
	return *this; 
}

std::string	Client::getNickname(void) const{
	return (_nickname);
}

std::string	Client::getUsername(void)const{
	return (_username);
}

std::string	Client::getHostname(void)const{
	return (_hostname);
}

int	Client::getSocketfd(void){
	return (_socket_fd);
}
