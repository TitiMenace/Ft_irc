#include <criterion/criterion.h>
#include <sys/wait.h>
#include "tests.hpp"

void test_response(const std::string &request, const std::string &expected_response, const std::string &password) {
	ServerProcess server(password);

	Client client(server.getPort());

	client.send(request);
	
	// Sleep 1 second
	usleep(1000000);
	
	client.expectResponse(expected_response);
}
