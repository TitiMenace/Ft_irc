
#include "includes.hpp"

Client::Client(void){}

Client::~Client(void){

	std::cout << "Client shutting down" << std::endl;
}

Client::Client(std::string nickname, std::string hostname, std::string username) : _nickname(nickname), _hostname(hostname), _username(username){
	_socket_fd = -1;
	std::cout << "Client registered" << std::endl;
}

Client::Client(const Client& copy){

	*this = copy;
}

Client &Client::operator=(const Client &type){

	_nickname = type.getNickname();
	_hostname = type.getHostname();
	_username = type.getUsername();
	_socket_fd = getSocketfd();
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
