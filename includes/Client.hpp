#pragma once

#include "includes.hpp"

class Client{


	private:
				std::string	_nickname;
				std::string	_hostname;
				std::string	_username;
				int			_socket_fd;
	public:
				Client();
				Client(int fd);
				Client(const Client& copy);
				~Client();
				Client& operator=(const Client& type);
		
				std::string	getNickname(void)const ;	
				std::string	getUsername(void)const ;	
				std::string	getHostname(void)const ;	
				int	getSocketfd(void);
				class ClientError : public std::exception{
			
				public:
				virtual const char* what() const throw()
				{
					return ("ClientError");
				}
	};
};

