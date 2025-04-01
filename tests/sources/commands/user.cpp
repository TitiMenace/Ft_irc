#include <criterion/criterion.h>
#include "tests.hpp"

Test(user, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER\r\n"
	);
	wait(0.1);
	client.expectResponse(
		"461 velimir USER :Not enough parameters\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(user, registration) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	);
	wait(0.1);
	client.expectResponse(
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is Buleubleubeuh, running version v0.1\r\n"
		"003 velimir :This server was created the 01/04/2025\r\n"
		"004 velimir :v0.1 + +tlkoi\r\n"
		"005 velimir CASEMAPPING=ascii :are supported by this server\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(user, already_registered) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	);
	wait(0.1);
	client.expectResponse(
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is Buleubleubeuh, running version v0.1\r\n"
		"003 velimir :This server was created the 01/04/2025\r\n"
		"004 velimir :v0.1 + +tlkoi\r\n"
		"005 velimir CASEMAPPING=ascii :are supported by this server\r\n"
	);

	client.send("USER velimir * 0 velimir\r\n");
	wait(0.1);
	client.expectResponse("462 velimir :You may not reregister\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}