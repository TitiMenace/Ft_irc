#include <criterion/criterion.h>
#include "tests.hpp"

Test(privmsg, no_password) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.send("PRIVMSG first :Hello first\r\n");
	wait(0.1);
	first.expectResponse("");
	second.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, not_registered) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.send(
		"PASS password\r\n"
		"PRIVMSG first :Hello first\r\n"
	);
	wait(0.1);
	first.expectResponse("");
	second.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, no_recipient) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("PRIVMSG\r\n");
	wait(0.1);
	client.expectResponse("411 client :No recipient given (PRIVMSG)\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, no_such_nick) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("PRIVMSG invalid :Hello client\r\n");
	wait(0.1);
	client.expectResponse("401 client invalid :No such nick/channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, no_such_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.register_("password", "client");
	client.send("PRIVMSG #invalid :Hello client\r\n");
	wait(0.1);
	client.expectResponse("401 client #invalid :No such nick/channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, no_text_to_send) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.register_("password", "second");
	second.send("PRIVMSG first\r\n");
	wait(0.1);
	first.expectResponse("");
	second.expectResponse("412 second :No text to send\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, direct_message) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());

	first.register_("password", "first");
	second.register_("password", "second");
	second.send("PRIVMSG first :Hello first\r\n");
	wait(0.1);
	first.expectResponse(":second PRIVMSG first :Hello first\r\n");
	second.expectResponse("");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, channel_message) try {
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
	second.expectResponse(
		":second!second@ JOIN #channel\r\n"
		"353 second = #channel :@first second\r\n"
		"366 second #channel :End of /NAMES list\r\n"
	);

	second.send("PRIVMSG #channel :Hello channel!\r\n");
	wait(0.1);
	first.expectResponse(":second PRIVMSG #channel :Hello channel!\r\n");
	second.expectResponse("");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(privmsg, external_channel_message) try {
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

	second.send("PRIVMSG #channel :Hello channel!\r\n");
	wait(0.1);
	first.expectResponse(":second PRIVMSG #channel :Hello channel!\r\n");
	second.expectResponse("");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
