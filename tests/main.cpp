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
	pid_t	server_pid;
	int	client_socket;
	
	server_pid = launch_server(3000, "blabla");
	try {
		client_socket = connect_to_server(3000);
	} catch (...) {
		stop_server(server_pid);
		throw;
	}

	std::string request = \
		"PASS blabla\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	;

	if (send(client_socket, request.c_str(), request.size(), 0) != request.size()) {
		stop_server(server_pid);
		throw std::runtime_error("Couldn't send the full request to the server");
	}
	
	usleep(1000000);

	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
	;

	std::string actual_reponse;
	actual_reponse.resize(expected_response.size());

	if (recv(client_socket, &actual_reponse[0], expected_response.size(), MSG_DONTWAIT) != expected_response.size()) {
		stop_server(server_pid);
		throw std::runtime_error("Couldn't receive the full response from the server");
	}
	
	if (expected_response != actual_reponse) {
		stop_server(server_pid);
		throw std::runtime_error(std::string("Response doesn't match what was expected: ") + actual_reponse);
	}

	std::cout << "OK" << std::endl;
	stop_server(server_pid);

	return 0;
} catch (std::runtime_error e) {
	std::cerr << "Error: " << e.what() << std::endl;
	return 1;
}
