#include <criterion/criterion.h>
#include "tests.hpp"

// Note:
// 	This is the usual format
// 		431 <client> :No nickname given
// 	Here client doesn't have a nickname yet.
// 	I thought it would make more sense to write * instead.
// 	Otherwise, if we omit <client> and send /quote NICK in irssi, it prints "nickname given" instead of "No nickname given".

Test(nick, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK\r\n"
	);
	wait(0.1);
	client.expectResponse(
		"461 * :No nickname given\r\n"
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
	wait(0.1);
	client.expectResponse(
		"432 * #velimir :Erroneus nickname\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(nick, nickname_in_use) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.send(
		"PASS password\r\n"
		"NICK first\r\n"
	);
	wait(0.1);
	second.send(
		"PASS password\r\n"
		"NICK first\r\n"
	);
	wait(0.1);
	second.expectResponse(
		"433 * first :Nickname is already in use.\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(nick, registration) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"USER velimir * 0 velimir\r\n"
		"NICK velimir\r\n"
	);
	wait(0.1);
	client.expectResponse(
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
		"005 velimir CASEMAPPING=ascii :are supported by this server\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
