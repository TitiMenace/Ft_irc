#include "tests.hpp"
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// Creates a TCP socket and connects it to a local server on port `port`
Client::Client(uint16_t port) {
	struct protoent		*protocol_entry;
	int					protocol;
	struct sockaddr_in	server_address;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("The client couldn't retrieve the TCP protocol");

	protocol = protocol_entry->p_proto;
	if ((_socket = socket(AF_INET, SOCK_STREAM, protocol)) == -1)
		throw std::runtime_error(std::string("The client couldn't create a socket: ") + strerror(errno));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (connect(_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(_socket);
		throw std::runtime_error(std::string("The client couldn't connect to the server: ") + strerror(errno));
	}
}

void Client::send(const std::string &request) {
	if (::send(_socket, request.c_str(), request.size(), 0) != (ssize_t) request.size())
		throw std::runtime_error("Couldn't send the full request to the server");
}

std::string Client::receive() {
	char buffer[RESPONSE_BUFFER_SIZE];
	std::string response;
	ssize_t chunk_size;

	while (true) {
		chunk_size = recv(_socket, buffer, RESPONSE_BUFFER_SIZE, MSG_DONTWAIT);
		if (chunk_size == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			throw std::runtime_error("Couldn't receive data from the server (possible crash)");
		if (chunk_size <= 0)
			break;
		response.append(buffer, chunk_size);
	}
	return response;
}

void Client::expectResponse(const std::string &expectedResponse) {
	std::string actualResponse = receive();
	if (actualResponse != expectedResponse) {
		throw std::runtime_error(
			std::string("Response doesn't match what was expected:\n")
			+ actualResponse +
			std::string("Instead of:\n")
			+ expectedResponse
		);
	}
}

Client::~Client() {
	close(_socket);
}