#include <criterion/criterion.h>
#include "tests.hpp"

Test(ping, pong) try {
	std::string request = \
		"PING :hello\r\n"
	;
	std::string expected_response = \
		"PONG :hello\r\n";

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
