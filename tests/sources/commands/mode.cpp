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
	wait(1);
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
	wait(1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK client\r\n"
		"USER client * 0 client\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 client :Welcome to the WiZ insane chat of distortion of reality between worlds, client!client@client\r\n"
		"002 client :Your host is , running version v.1\r\n"
		"003 client :This server was created le 01/01/01\r\n"
		"004 client :v.1 no user mode support +tlkoiq\r\n"
	);

	client.send("MODE\r\n");
	wait(1);
	client.expectResponse("461 client MODE :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, no_such_channel) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
		"JOIN #room1\r\n"
		"MODE dani\r\n"
	;
	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
		":velimir!velimir@ JOIN #room1\r\n"
		"353 velimir = #room1 :@velimir\r\n"
		"366 velimir #room1 :End of /NAMES list\r\n"
		"403 velimir dani :No such channel\r\n"
		;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
