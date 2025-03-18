#include <criterion/criterion.h>
#include "tests.hpp"

Test(join, not_registered) try {
	std::string request = \
		"JOIN popo\r\n"
	;
	std::string expected_response = \
		"451 * :You have not registered\r\n";

	test_response(request, expected_response, "password");
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
