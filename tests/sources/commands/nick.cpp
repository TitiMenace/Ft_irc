#include <criterion/criterion.h>
#include "tests.hpp"


Test(nick, registration) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"USER velimir * 0 velimir\r\n"
		"NICK velimir\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(nick, erroneous_nickname) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK #velimir\r\n"
	);
	wait(1);
	client.expectResponse(
		"432 :Erroneus nickname\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(nick, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK\r\n"
	);
	wait(1);
	client.expectResponse(
		"461 :No nickname given\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
