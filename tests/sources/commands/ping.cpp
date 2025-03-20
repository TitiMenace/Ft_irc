#include <criterion/criterion.h>
#include "tests.hpp"

Test(ping, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());
	
	client.register_("password", "client");
	client.send("PING\r\n");
	wait(0.1);
	client.expectResponse("461 client PING :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(ping, pong) try {
	ServerProcess server("password");
	Client client(server.getPort());
	
	client.register_("password", "client");
	client.send("PING :hello\r\n");
	wait(0.1);
	client.expectResponse("PONG server :hello\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
