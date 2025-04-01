#include <criterion/criterion.h>
#include <stdexcept>
#include "tests.hpp"

Test(mode, missing_key_param) try {
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

	client.send("MODE #channel +k\r\n");
	wait(0.1);
	client.expectResponse("461 client MODE :Missing <key> parameter\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, invalid_key_param) try {
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

	client.send("MODE #channel +k :Contains space\r\n");
	wait(0.1);
	client.expectResponse("525 client #channel :Key is not well-formed\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, valid_key) try {
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

	client.send("MODE #channel +k :key\r\n");
	wait(0.1);
	client.expectResponse(":client MODE #channel +k :key\r\n");

	client.send("MODE #channel -k\r\n");
	wait(0.1);
	client.expectResponse(":client MODE #channel -k\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
