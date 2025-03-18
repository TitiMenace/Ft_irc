#include <criterion/criterion.h>
#include "tests.hpp"

Test(join, no_password) try {
	std::string request = \
		"JOIN #popo\r\n"
	;
	std::string expected_response = \
		"451 * :You have not registered\r\n";

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, not_registered) try {
	std::string request = \
		"PASS password\r\n"
		"JOIN #popo\r\n"
	;
	std::string expected_response = \
		"451 * :You have not registered\r\n";

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, not_enough_params) try {
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

	client.send("JOIN\r\n");
	wait(1);
	client.expectResponse("461 client JOIN :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, invalid_channel) try {
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

	client.send("JOIN channel\r\n");
	wait(1);
	client.expectResponse("476 channel :Bad Channel Mask\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}