#include <criterion/criterion.h>
#include <stdexcept>
#include "tests.hpp"

Test(mode, no_user_param) try {
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

	client.send("MODE #channel +o\r\n");
	wait(0.1);
	client.expectResponse("461 client MODE :Missing <member> parameter\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, target_not_in_network) try {
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

	client.send("MODE #channel +o not_here\r\n");
	wait(0.1);
	client.expectResponse("401 client not_here :No such nick/channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, target_not_in_channel) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.register_("password", "second");

	first.send("JOIN #channel\r\n");
	wait(0.1);
	first.expectResponse(
		":first!first@ JOIN #channel\r\n"
		"353 first = #channel :@first\r\n"
		"366 first #channel :End of /NAMES list\r\n"
	);

	first.send("MODE #channel +o second\r\n");
	wait(0.1);
	first.expectResponse("441 first second #channel :They aren't on that channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}