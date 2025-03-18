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

	client.register_("password", "client");
	client.send("JOIN\r\n");
	wait(0.1);
	client.expectResponse("461 client JOIN :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, invalid_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("JOIN channel\r\n");
	wait(0.1);
	client.expectResponse("476 channel :Bad Channel Mask\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, success) try {
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
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}