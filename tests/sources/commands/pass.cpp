#include <criterion/criterion.h>
#include "tests.hpp"


Test(pass, password_mismatch) try {
	std::string request = \
		"PASS invalid\r\n"
	;

	std::string expected_response = \
		"464 * :Password incorrect\r\n"
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
		"461 * PASS :Not enough parameters\r\n";
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
		"002 velimir :Your host is Buleubleubeuh, running version v0.1\r\n"
		"003 velimir :This server was created the 01/04/2025\r\n"
		"004 velimir :v0.1 + +tlkoi\r\n"
		"005 velimir CASEMAPPING=ascii :are supported by this server\r\n"
		"462 velimir :You may not reregister\r\n"
	;

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
