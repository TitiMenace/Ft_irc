#include <criterion/criterion.h>
#include <stdexcept>
#include "tests.hpp"

Test(mode, valid_protected_topic) try {
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

	client.send("MODE #channel +t\r\n");
	wait(0.1);
	client.expectResponse(":client MODE #channel +t\r\n");

	client.send("MODE #channel -t\r\n");
	wait(0.1);
	client.expectResponse(":client MODE #channel -t\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
