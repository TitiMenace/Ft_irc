
#include "Client.hpp"

Client::Client(void){}

Client::~Client(void){}

Client::Client(std::string nickname): _nickname(nickname){

		_fd = -1;
		std::cout << "Creation de Client" << std::endl;
}

Client::Client(const Client& copy){

	*this = copy;
}

Client &Client::operator=(const Client &type){

	_nickname = type.getNickname();
	return *this; 
}

std::string	Client::getNickname(void) const{
	return (_nickname);
}

void	Client::setFd(int fd){
	_fd = fd;
	return;
}
