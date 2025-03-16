#include <criterion/criterion.h>
#include "tests.hpp"


Test(nick, registration) try {
	std::string request = \
		"PASS password\r\n"
		"USER velimir * 0 velimir\r\n"
		"NICK velimir\r\n"
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
