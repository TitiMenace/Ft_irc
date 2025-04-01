#include <criterion/criterion.h>
#include "tests.hpp"

Test(topic, not_allowed) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");

	first.send("JOIN #channel\r\n");
	wait(0.1);
	first.expectResponse(
		":first!first@ JOIN #channel\r\n"
		"353 first = #channel :@first\r\n"
		"366 first #channel :End of /NAMES list\r\n"
	);

	second.send("TOPIC #channel\r\n");
	wait(0.1);
	second.expectResponse("451 * :You have not registered\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(topic, not_registered) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.send("PASS password\r\n");

	first.send("JOIN #channel\r\n");
	wait(0.1);
	first.expectResponse(
		":first!first@ JOIN #channel\r\n"
		"353 first = #channel :@first\r\n"
		"366 first #channel :End of /NAMES list\r\n"
	);

	second.send("TOPIC #channel\r\n");
	wait(0.1);
	second.expectResponse("451 * :You have not registered\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(topic, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");

	client.send("TOPIC\r\n");
	wait(0.1);
	client.expectResponse("461 client TOPIC :Not enough parameters\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(topic, no_such_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");

	client.send("TOPIC #invalid\r\n");
	wait(0.1);
	client.expectResponse("403 client #invalid :No such channel\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(topic, valid_get_topic) try {
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
	client.send("TOPIC #channel\r\n");
	wait(0.1);
	client.expectResponse("332 client #channel :topic\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(topic, valid_set_topic) try {
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

	first.send("TOPIC #channel :new topic\r\n");
	wait(0.1);
	first.expectResponse(":first TOPIC #channel :new topic\r\n");
	second.expectResponse(":first TOPIC #channel :new topic\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(topic, protected_topic) try {
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

	first.send("MODE #channel +t\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +t\r\n");
	second.expectResponse(":first MODE #channel +t\r\n");

	second.send("TOPIC #channel topic\r\n");
	wait(0.1);
	second.expectResponse("482 second #channel :You're not channel operator\r\n");

	first.send("TOPIC #channel topic\r\n");
	wait(0.1);
	first.expectResponse(":first TOPIC #channel :topic\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
