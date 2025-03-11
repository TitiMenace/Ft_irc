#include <criterion/criterion.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "tests.hpp"

void test_response(const std::string &request, const std::string &expected_response, uint16_t port, const std::string &password) {
	pid_t	server_pid;
	int		client_socket;

	server_pid = launch_server(port, password);
	try {
		client_socket = connect_to_server(port);
	} catch (...) {
		stop_server(server_pid);
		throw;
	}

	if (send(client_socket, request.c_str(), request.size(), 0) != (ssize_t) request.size()) {
		stop_server(server_pid);
		throw std::runtime_error("Couldn't send the full request to the server");
	}
	
	// Sleep 1 second
	usleep(1000000);

	std::string actual_reponse;
	actual_reponse.resize(expected_response.size());

	ssize_t nbBytes = recv(client_socket, &actual_reponse[0], expected_response.size(), MSG_DONTWAIT);
	if (nbBytes == -1) {
		stop_server(server_pid);
		throw std::runtime_error("Couldn't receive data from the server (possible crash)");
	}
	actual_reponse.resize(nbBytes);
	
	if (actual_reponse != expected_response.substr(0, nbBytes)) {
		stop_server(server_pid);
		throw std::runtime_error(std::string("Response doesn't match what was expected: ") + actual_reponse);
	}
	
	if (actual_reponse.size() != expected_response.size()) {
		stop_server(server_pid);
		throw std::runtime_error("Truncated response received. Maybe increase the response delay.");
	}

	stop_server(server_pid);
}


Test(registration, valid) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	;

	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir : v.1 no user mode support +tlkoiq\r\n"
	;

	test_response(request, expected_response, 8080, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
