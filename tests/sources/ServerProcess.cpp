#include "tests.hpp"
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

std::string u16_to_string(uint16_t port) {
	std::stringstream stream;
	stream << port;
	return stream.str();
}

// Launches the ircsrv executable in a separate process.
// If the launch failed, the process will exit with status 125.
// TODO: use pipes to detect a failed launch instead
ServerProcess::ServerProcess(const std::string &password) : _port(find_available_port())
{
	int log_fd = open("/dev/null", O_WRONLY | O_CLOEXEC);
	if (log_fd == -1)
		throw std::runtime_error("Couldn't open log file for server");

	_pid = fork();
	if (_pid == -1)
		throw std::runtime_error(
			"Server process creation failed\n"
			"Note: Check the process limit"
		);
	if (_pid == 0)
	{
		extern char **environ;
		dup2(log_fd, STDOUT_FILENO);
		dup2(log_fd, STDERR_FILENO);
		execve("../ircserv",
			(char *[]){
				(char *) "../ircserv",
				(char *) u16_to_string(_port).c_str(),
				(char *) password.c_str(),
				NULL
			},
			environ
		);
		exit(125);
	}
	close(log_fd);
	usleep(100000);

	int wstatus;
	if (waitpid(_pid, &wstatus, WNOHANG) == _pid
		&& WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 125)
		throw std::runtime_error(
			"Couldn't find or launch the server\n"
			"Note: Check the server's permissions and executable format"
		);
}

uint16_t ServerProcess::find_available_port() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;  // Let OS pick an available port

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sock);
        throw std::runtime_error("Failed to bind socket");
    }

    socklen_t addr_len = sizeof(addr);
    if (getsockname(sock, (struct sockaddr*)&addr, &addr_len) == -1) {
        close(sock);
        throw std::runtime_error("Failed to get socket name");
    }

    uint16_t port = ntohs(addr.sin_port);
    close(sock);  // Close the socket after getting the port
    return port;
}

ServerProcess::~ServerProcess()
{
	int wstatus;

	kill(_pid, SIGTERM);
	waitpid(_pid, &wstatus, 0);

	// TODO: check if server closed successfuly
}

uint16_t ServerProcess::getPort() {
	return _port;
}