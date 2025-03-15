#pragma once

#include <sys/types.h>
#include <stdexcept>
#define RESPONSE_BUFFER_SIZE 1024

class ServerProcess {
	int			_pid;

public:
	ServerProcess(uint16_t port, const std::string &password);
	~ServerProcess();
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