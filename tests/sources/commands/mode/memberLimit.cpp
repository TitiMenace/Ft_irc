#include <criterion/criterion.h>
#include <stdexcept>
#include "tests.hpp"

Test(mode, missing_user_limit_param) try {
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

	client.send("MODE #channel +l\r\n");
	wait(0.1);
	client.expectResponse("461 client MODE :Missing <limit> parameter\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, valid_member_limit) try {
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

	client.send("MODE #channel +l 2\r\n");
	wait(0.1);
	client.expectResponse(":client MODE #channel +l :2\r\n");

	client.send("MODE #channel -l\r\n");
	wait(0.1);
	client.expectResponse(":client MODE #channel -l\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
