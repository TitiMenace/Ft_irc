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

// Note: this is called register_ instead of register, because register is a C++ keyword and as such is not allowed as a function name
void Client::register_(const std::string &password, const std::string &nickname) {
	send(
		"PASS " + password + "\r\n"
		"NICK " + nickname + "\r\n"
		"USER " + nickname + " * 0 " + nickname + "\r\n"
	);
	wait(0.1);
	expectResponse(
		"001 " + nickname + " :Welcome to the WiZ insane chat of distortion of reality between worlds, " + nickname + "!" + nickname + "@" + nickname + "\r\n"
		"002 " + nickname + " :Your host is Buleubleubeuh, running version v0.1\r\n"
		"003 " + nickname + " :This server was created the 01/04/2025\r\n"
		"004 " + nickname + " :v0.1 + +tlkoi\r\n"
		"005 " + nickname + " CASEMAPPING=ascii :are supported by this server\r\n"
	);
}

Client::~Client() {
	close(_socket);
}