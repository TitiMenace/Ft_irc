#include <criterion/criterion.h>
#include "tests.hpp"

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
