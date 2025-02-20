#pragma once


#define	SA struct sockaddr

#include "includes.hpp"

class Server{

	
	private:
				struct	sockaddr_in	servaddr;
				int	addrlen;
				int	sock_fd;
				int socket_in;

	public:
		Server();
		Server(int port);
		Server(const Server& copy);
		~Server();
		Server& operator=(const Server& type);
		
		void	runServer(void);
		bool	bindingStream(void);
		void	inputLoop(void);

		class Aziz : public std::exception{
		
			public:
				virtual const char* what() const throw()
				{
					return ("aziz");
				}
	};
};

