#include <criterion/criterion.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tests.hpp"

Test(mode, no_password) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send("MODE #channel +i\r\n");
	wait(1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, not_registered) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"MODE #channel +i\r\n"
	);
	wait(1);
	client.expectResponse("451 * :You have not registered\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, not_enough_params) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK client\r\n"
		"USER client * 0 client\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 client :Welcome to the WiZ insane chat of distortion of reality between worlds, client!client@client\r\n"
		"002 client :Your host is , running version v.1\r\n"
		"003 client :This server was created le 01/01/01\r\n"
		"004 client :v.1 no user mode support +tlkoiq\r\n"
	);

	client.send("MODE\r\n");
	wait(1);
	client.expectResponse("461 client MODE :Not enough parameters\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, invalid_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
	);
	
	client.send("MODE dani\r\n");
	wait(1);
	client.expectResponse("403 velimir dani :No such channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, no_such_channel) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
	);
	
	client.send("MODE #dani\r\n");
	wait(1);
	client.expectResponse("403 velimir #dani :No such channel\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, mode_list) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK client\r\n"
		"USER client * 0 client\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 client :Welcome to the WiZ insane chat of distortion of reality between worlds, client!client@client\r\n"
		"002 client :Your host is , running version v.1\r\n"
		"003 client :This server was created le 01/01/01\r\n"
		"004 client :v.1 no user mode support +tlkoiq\r\n"
	);

	client.send("JOIN #channel\r\n");
	wait(1);
	client.expectResponse(
		":client!client@ JOIN #channel\r\n"
		"353 client = #channel :@client\r\n"
		"366 client #channel :End of /NAMES list\r\n"
	);
	
	client.send("MODE #channel\r\n");
	wait(1);
	client.expectResponse("324 client #channel :o \r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(mode, unknown_mode) try {
	ServerProcess server("password");
	Client client(server.getPort());

	client.send(
		"PASS password\r\n"
		"NICK client\r\n"
		"USER client * 0 client\r\n"
	);
	wait(1);
	client.expectResponse(
		"001 client :Welcome to the WiZ insane chat of distortion of reality between worlds, client!client@client\r\n"
		"002 client :Your host is , running version v.1\r\n"
		"003 client :This server was created le 01/01/01\r\n"
		"004 client :v.1 no user mode support +tlkoiq\r\n"
	);

	client.send("JOIN #channel\r\n");
	wait(1);
	client.expectResponse(
		":client!client@ JOIN #channel\r\n"
		"353 client = #channel :@client\r\n"
		"366 client #channel :End of /NAMES list\r\n"
	);
	
	client.send("MODE #channel +g\r\n");
	wait(1);
	client.expectResponse("501 client :Unknown MODE flag (g)\r\n");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}