#include <criterion/criterion.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tests.hpp"

Test(mode, no_password) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send("MODE #channel +i\r\n");
	wait(0.1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, not_registered) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"MODE #channel +i\r\n"
	);
	wait(0.1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("MODE\r\n");
	wait(0.1);
	client.expectResponse("461 client MODE :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, invalid_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "velimir");
	client.send("MODE dani\r\n");
	wait(0.1);
	client.expectResponse("403 velimir dani :No such channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, no_such_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "velimir");
	client.send("MODE #dani\r\n");
	wait(0.1);
	client.expectResponse("403 velimir #dani :No such channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, mode_list) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("JOIN #channel\r\n");
	wait(0.1);
	client.expectResponse(
		":client!client@ JOIN #channel\r\n"
		"353 client = #channel :@client\r\n"
		"366 client #channel :End of /NAMES list\r\n"
	);
	
	client.send("MODE #channel\r\n");
	wait(0.1);
	client.expectResponse("324 client #channel :o \r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, unknown_mode) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("JOIN #channel\r\n");
	wait(0.1);
	client.expectResponse(
		":client!client@ JOIN #channel\r\n"
		"353 client = #channel :@client\r\n"
		"366 client #channel :End of /NAMES list\r\n"
	);
	
	client.send("MODE #channel +g\r\n");
	wait(0.1);
	client.expectResponse("501 client :Unknown MODE flag (g)\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}