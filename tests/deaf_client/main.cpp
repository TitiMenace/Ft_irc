#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tests.hpp"

// Creates a TCP socket and connects it to a local server on port `port`

int	connect_to_server(uint16_t port) {
	struct protoent		*protocol_entry;
	int					protocol;
	struct sockaddr_in	server_address;
	int					client_fd;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("The client couldn't retrieve the TCP protocol");

	protocol = protocol_entry->p_proto;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, protocol)) == -1)
		throw std::runtime_error(std::string("The client couldn't create a socket: ") + strerror(errno));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(client_fd);
		throw std::runtime_error(std::string("The client couldn't connect to the server: ") + strerror(errno));
	}
	return client_fd;
}


int	main() try {
	int	client_socket;
	
	try {
		client_socket = connect_to_server(3000);
	} catch (...) {
		throw;
	}

	std::string request = \
		"PASS blabla\r\n"
		"NICK velomir\r\n"
		"USER velomir * 0 velomir\r\n"
	;

	if (send(client_socket, request.c_str(), request.size(), 0) != request.size())
		throw std::runtime_error("Couldn't send the full request to the server");
	
	usleep(1000000);
	shutdown(client_socket, SHUT_RD);
	std::cout << "Read end has been shut down" << std::endl;

	char c[1];
	read(STDIN_FILENO, c, 1);

	return 0;
} catch (std::runtime_error e) {
	std::cerr << "Error: " << e.what() << std::endl;
	return 1;
}
