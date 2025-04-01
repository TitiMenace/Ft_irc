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
