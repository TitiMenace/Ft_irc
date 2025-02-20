#pragma once


#define	SA struct sockaddr

#include "includes.hpp"

class Client{

	private:	
				int				_fd;
				std::string		_nickname;
				//std::string		_username;
				//std::string		_realname;

	public:	
		
				Client();
				Client(std::string nickname);
				Client(const Client &copy);
				~Client();
				Client& operator=(const Client& type);

				std::string	getNickname(void) const;
				void		setFd(int fd);
		
		class Aziz : public std::exception{
		
			public:
				virtual const char* what() const throw()
				{
					return ("aziz");
				}
	};
};

