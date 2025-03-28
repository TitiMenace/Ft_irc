#include <criterion/criterion.h>
#include "tests.hpp"

Test(invite, no_password) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.send("INVITE first #channel\r\n");
	wait(0.1);
	first.expectResponse("");
	second.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, not_registered) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.send(
		"PASS password\r\n"
		"INVITE first #channel\r\n"
	);
	wait(0.1);
	first.expectResponse("");
	second.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, no_target_param) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("INVITE\r\n");
	wait(0.1);
	client.expectResponse("461 client INVITE :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, no_channel_param) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.register_("password", "second");
	first.send("INVITE second\r\n");
	wait(0.1);
	first.expectResponse("461 first INVITE :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, inviting) try {
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

	first.send("INVITE second #channel\r\n");
	wait(0.1);
	first.expectResponse("341 first second #channel\r\n");
	second.expectResponse(":first INVITE second #channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
