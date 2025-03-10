#pragma once


#define	SA struct sockaddr

#include "includes.hpp"
#include <map>
#include <vector>
#include "Message.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ERR_MSG.hpp"
class Server{
	
	private:
		struct	sockaddr_in	_servaddr;
		int	_addrlen;
		int	_master_fd;
		std::string	_password;
		std::map<int, Client> _users;
		std::map<std::string, Channel> _channel_list;
		
		void	_acceptClient(int epoll_fd);
		bool	_runCommand(Client &client, std::string &buffer, std::size_t &buffer_pos);
		void	_readMessages(struct epoll_event event);
		void	_disconnectClient(Client &client);

	public:
		Server();
		Server(int port, std::string password);
		Server(const Server& copy);
		~Server();
		Server& operator=(const Server& type);
		
		void	runServer(void);
		bool	bindingStream(void);

		void nick(Message message, Client &client);
		void user(Message message, Client &client);
		void pass(Message message, Client &client);
		void join(Message message, Client &client);
		void ping(Message message, Client &client);
		void kick(Message message, Client &client);
		void mode(Message message, Client &client);
		void topic(Message message, Client &client);
		void invite(Message message, Client &client);
		void privmsg(Message message, Client &client); 
		Client* findUser(std::string nickname);
		void channelflagsGestion(Message message, Client &client);

	
		class Aziz : public std::exception{
		
			public:
				virtual const char* what() const throw()
				{
					return ("aziz");
				}
		};
};

