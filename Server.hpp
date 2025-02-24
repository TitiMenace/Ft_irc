#pragma once


#define	SA struct sockaddr

#include "includes.hpp"

class Server{

	
	private:
				struct	sockaddr_in	_servaddr;
				int	_addrlen;
				int	_master_fd;
				std::string	_password;

	public:
		Server();
		Server(int port, std::string password);
		Server(const Server& copy);
		~Server();
		Server& operator=(const Server& type);
		
		void	runServer(void);
		bool	bindingStream(void);

		class Aziz : public std::exception{
		
			public:
				virtual const char* what() const throw()
				{
					return ("aziz");
				}
	};
};

