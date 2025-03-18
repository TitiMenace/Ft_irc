#include <criterion/criterion.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "tests.hpp"

void wait(float seconds) {
	usleep(seconds * 1000000);
}

void test_response(const std::string &request, const std::string &expected_response, const std::string &password) {
	ServerProcess server(password);

	Client client(server.getPort());

	client.send(request);
	
	wait(0.1);
	
	client.expectResponse(expected_response);
}
