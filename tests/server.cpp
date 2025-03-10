#include <stdexcept>
#include <sstream>
#include <cstdlib>
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
pid_t	launch_server(uint16_t port, const std::string &password) throw (std::runtime_error)
{
	int log_fd = open("/dev/null", O_WRONLY | O_CLOEXEC);
	if (log_fd == -1)
		throw std::runtime_error("Couldn't open log file for server");

	pid_t server_pid = fork();
	if (server_pid == -1)
		throw std::runtime_error(
			"Server process creation failed\n"
			"Note: Check the process limit"
		);
	if (server_pid == 0)
	{
		extern char **environ;
		dup2(log_fd, STDOUT_FILENO);
		dup2(log_fd, STDERR_FILENO);
		execve("../ircserv",
			(char *[]){
				(char *) "../ircserv",
				(char *) u16_to_string(port).c_str(),
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
	if (waitpid(server_pid, &wstatus, WNOHANG) == server_pid
		&& WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 125)
		throw std::runtime_error(
			"Couldn't find or launch the server\n"
			"Note: Check the server's permissions and executable format"
		);
	return server_pid;
}

void	stop_server(pid_t server_pid) throw(std::runtime_error)
{
	int wstatus;

	kill(server_pid, SIGTERM);
	waitpid(server_pid, &wstatus, 0);

	// TODO: check for status 0 once ircserv fails gracefully
	if (!WIFSIGNALED(wstatus) || WTERMSIG(wstatus) != SIGTERM)
		throw std::runtime_error("The server reported an error");
}
