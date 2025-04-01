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

Test(invite, no_such_nick) try {
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

	client.send("INVITE invalid #channel\r\n");
	wait(0.1);
	client.expectResponse("401 client invalid :No such nick/channel\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, no_such_channel) try {
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

	first.send("INVITE second #invalid\r\n");
	wait(0.1);
	first.expectResponse("403 first #invalid :No such channel\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, not_in_channel) try {
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

	second.send("INVITE second #channel\r\n");
	wait(0.1);
	second.expectResponse("442 second #channel :You're not on that channel\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(invite, already_in_channel) try {
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

	first.send("INVITE first #channel\r\n");
	wait(0.1);
	first.expectResponse("443 first first #channel :is already on channel\r\n");

} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(invite, invite_only_channel) try {
	ServerProcess server("password");
	Client first(server.getPort());
	Client second(server.getPort());
	Client third(server.getPort());

	first.register_("password", "first");
	second.register_("password", "second");
	third.register_("password", "third");

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


	first.send("MODE #channel +i\r\n");
	wait(0.1);
	first.expectResponse(":first MODE #channel +i\r\n");
	second.expectResponse(":first MODE #channel +i\r\n");

	second.send("INVITE third #channel\r\n");
	wait(0.1);

	second.expectResponse("482 second #channel :You're not channel operator\r\n");

	first.send("INVITE third #channel\r\n");
	wait(0.1);
	first.expectResponse("341 first third #channel\r\n");


} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
