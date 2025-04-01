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

Test(join, invite_only) try {
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

	first.send("MODE #channel +i\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +i\r\n");

	second.send("JOIN #channel\r\n");
	wait(0.1);
	second.expectResponse("473 second #channel :Cannot join channel (+i)\r\n");

	first.send("INVITE second #channel\r\n");
	wait(0.1);
	first.expectResponse("341 first second #channel\r\n");
	second.expectResponse(":first INVITE second #channel\r\n");

	second.send("JOIN #channel\r\n");
	wait(0.1);
	second.expectResponse(
		":second!second@ JOIN #channel\r\n"
		"353 second = #channel :@first second\r\n"
		"366 second #channel :End of /NAMES list\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, key_only) try {
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

	first.send("MODE #channel +k :key\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +k :key\r\n");

	second.send("JOIN #channel\r\n");
	wait(0.1);
	second.expectResponse("475 second #channel :Cannot join channel (+k)\r\n");

	first.send("MODE #channel +k :goodkey\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +k :goodkey\r\n");

	second.send("JOIN #channel goodkey\r\n");
	wait(0.1);
	second.expectResponse(
		":second!second@ JOIN #channel\r\n"
		"353 second = #channel :@first second\r\n"
		"366 second #channel :End of /NAMES list\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(join, user_limit) try {
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

	first.send("MODE #channel +l 1\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +l :1\r\n");

	second.send("JOIN #channel\r\n");
	wait(0.1);
	second.expectResponse("471 second #channel :Cannot join channel (+l)\r\n");

	first.send("MODE #channel +l 2\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +l :2\r\n");

	second.send("JOIN #channel\r\n");
	wait(0.1);
	second.expectResponse(
		":second!second@ JOIN #channel\r\n"
		"353 second = #channel :@first second\r\n"
		"366 second #channel :End of /NAMES list\r\n"
	);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
