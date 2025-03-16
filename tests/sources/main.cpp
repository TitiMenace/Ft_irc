#include <criterion/criterion.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tests.hpp"

void test_response(const std::string &request, const std::string &expected_response, const std::string &password) {
	ServerProcess server(password);

	Client client(server.getPort());

	client.send(request);
	
	// Sleep 1 second
	usleep(1000000);
	
	client.expectResponse(expected_response);
}

Test(registration, valid) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
	;
	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(join, notregistered) try {
	std::string request = \
		"JOIN popo\r\n"
	;
	std::string expected_response = \
		"451 :You have not registered\r\n";

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(pass, password_mismatch) try {
	std::string request = \
		"PASS invalid\r\n"
	;

	std::string expected_response = \
		"464 :Password incorrect\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(Mode, test) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
		"JOIN #room1\r\n"
		"MODE dani\r\n"
	;
	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
		":velimir!velimir@ JOIN #room1\r\n"
		"353 velimir = #room1 : @velimir\r\n"
		"366 velimir #room1 :End of /NAMES list\r\n"
		"403 velimir dani :No such channel\r\n"
		;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}



Test(pass, not_enough_params) try {
	std::string request = \
		"PASS\r\n"
	;

	std::string expected_response = \
	"461 PASS :Not enough parameters\r\n";
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(pass, already_registered) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER velimir * 0 velimir\r\n"
		"PASS password\r\n"
	;

	std::string expected_response = \
		"001 velimir :Welcome to the WiZ insane chat of distortion of reality between worlds, velimir!velimir@velimir\r\n"
		"002 velimir :Your host is , running version v.1\r\n"
		"003 velimir :This server was created le 01/01/01\r\n"
		"004 velimir :v.1 no user mode support +tlkoiq\r\n"
		"462 velimir :You may not reregister\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(nick, erroneous_nickname) try {
	std::string request = \
		"PASS password\r\n"
		"NICK #velimir\r\n"
	;

	std::string expected_response = \
		"432 :Erroneus nickname\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}


Test(nick, not_enough_params) try {
	std::string request = \
		"PASS password\r\n"
		"NICK\r\n"
	;

	std::string expected_response = \
		"461 :No nickname given\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

Test(user, not_enough_params) try {
	std::string request = \
		"PASS password\r\n"
		"NICK velimir\r\n"
		"USER\r\n"
	;

	std::string expected_response = \
		"461 velimir USER :Not enough parameters\r\n";
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
