#pragma once

#include <string>
#include <cstdint>

#define RESPONSE_BUFFER_SIZE 1024

void wait(float seconds);
void test_response(const std::string &request, const std::string &expected_response, const std::string &password);

class ServerProcess {
	uint16_t	_port;
	int			_pid;

	static uint16_t find_available_port();

public:
	ServerProcess(const std::string &password);
	~ServerProcess();

	uint16_t getPort();
};

class Client {
	int	_socket;

public:
	Client(uint16_t port);
	~Client();
	void send(const std::string &request);
	std::string receive();
	void expectResponse(const std::string &expectedResponse);
};