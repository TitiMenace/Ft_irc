#include <criterion/criterion.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tests.hpp"
uint16_t find_available_port() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;  // Let OS pick an available port

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sock);
        throw std::runtime_error("Failed to bind socket");
    }

    socklen_t addr_len = sizeof(addr);
    if (getsockname(sock, (struct sockaddr*)&addr, &addr_len) == -1) {
        close(sock);
        throw std::runtime_error("Failed to get socket name");
    }

    uint16_t port = ntohs(addr.sin_port);
    close(sock);  // Close the socket after getting the port
    return port;
}

void test_response(const std::string &request, const std::string &expected_response, const std::string &password) {
	pid_t	server_pid;
	int		client_socket;
	uint16_t port_test = find_available_port();
	//how to check available port
	//assign avaiable port to shiet
	server_pid = launch_server(port_test, password);
	try {
		client_socket = connect_to_server(port_test);
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

		throw std::runtime_error(std::string("Truncated response received. Maybe increase the response delay.") + actual_reponse);
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
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(pass, password_mismatch) try {
	std::string request = \
		"PASS invalid\r\n"
	;

	std::string expected_response = \
		"464 :Password incorrect\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(pass, not_enough_params) try {
	std::string request = \
		"PASS\r\n"
	;

	std::string expected_response = \
	"461 PASS :Not enough parameters\r\n";
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(pass, already_registered) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
		"PASS password\r\n"
	;

	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
		"462 velimir :You may not reregister\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(nick, erroneous_nickname) try {
	std::string request = \
		"PASS password\r\n"
		"NICK #velimir\r\n"
	;

	std::string expected_response = \
		"432 :Erroneus nickname\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(nick, not_enough_params) try {
	std::string request = \
		"PASS password\r\n"
		"NICK\r\n"
	;

	std::string expected_response = \
		"461 :No nickname given\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(user, not_enough_params) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER\r\n"
	;

	std::string expected_response = \
		"461 velimir USER :Not enough parameters\r\n";
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
