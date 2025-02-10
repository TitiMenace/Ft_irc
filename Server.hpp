#pragma once

#include "includes.hpp"

class Server{

	private:
				int *fds;
		
	public:
		Server();
		Server(unsigned int);
		Server(const Server& copy);
		~Server();
		Server& operator=(const Server& type);
		
		void	runServer(void);
		class Aziz : public std::exception{
		
			public:
				virtual const char* what() const throw()
				{
					return ("aziz");
				}
	};
};

