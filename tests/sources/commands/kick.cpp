#include <criterion/criterion.h>
#include "tests.hpp"

Test(kick, no_password) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send("KICK #channel invalid\r\n");
	wait(0.1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, not_registered) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"KICK #channel invalid\r\n"
	);
	wait(0.1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, no_channel_param) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");

	client.send("KICK\r\n");
	wait(0.1);
	client.expectResponse("461 client KICK :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, no_such_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");

	client.send("KICK #invalid client\r\n");
	wait(0.1);
	client.expectResponse("403 client #invalid :No such channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, not_in_channel) try {
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

	second.send("KICK #channel second\r\n");
	wait(0.1);
	second.expectResponse("442 second #channel :You're not on that channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, no_user_param) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");

	client.send("KICK #channel\r\n");
	wait(0.1);
	client.expectResponse("461 client KICK :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, invalid_user) try {
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

	client.send("KICK #channel invalid\r\n");
	wait(0.1);
	client.expectResponse("401 client invalid :No such nick/channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(kick, user_not_in_channel) try {
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

	first.send("KICK #channel second\r\n");
	wait(0.1);
	first.expectResponse("441 first second #channel :They aren't on that channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, not_op) try {
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

	second.send("JOIN #channel\r\n");
	wait(0.1);
	first.expectResponse(":second!second@ JOIN #channel\r\n");
	second.expectResponse(
		":second!second@ JOIN #channel\r\n"
		"353 second = #channel :@first second\r\n"
		"366 second #channel :End of /NAMES list\r\n"
	);

	second.send("KICK #channel second\r\n");
	wait(0.1);
	second.expectResponse("482 second #channel :You're not channel operator\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(kick, valid) try {
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

	second.send("JOIN #channel\r\n");
	wait(0.1);
	first.expectResponse(":second!second@ JOIN #channel\r\n");
	second.expectResponse(
		":second!second@ JOIN #channel\r\n"
		"353 second = #channel :@first second\r\n"
		"366 second #channel :End of /NAMES list\r\n"
	);

	first.send("KICK #channel second\r\n");
	wait(0.1);
	second.expectResponse(":first KICK #channel second :No particular reason.\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
