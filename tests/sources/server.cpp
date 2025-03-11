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
pid_t	launch_server(uint16_t port, const std::string &password)
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

// Creates a TCP socket and connects it to a local server on port `port`
int	connect_to_server(uint16_t port) {
	struct protoent		*protocol_entry;
	int					protocol;
	struct sockaddr_in	server_address;
	int					client_fd;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("The client couldn't retrieve the TCP protocol");

	protocol = protocol_entry->p_proto;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, protocol)) == -1)
		throw std::runtime_error(std::string("The client couldn't create a socket: ") + strerror(errno));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(client_fd);
		throw std::runtime_error(std::string("The client couldn't connect to the server: ") + strerror(errno));
	}
	return client_fd;
}

void	stop_server(pid_t server_pid)
{
	int wstatus;

	kill(server_pid, SIGTERM);
	waitpid(server_pid, &wstatus, 0);

	// TODO: check for status 0 once ircserv fails gracefully
	if (!WIFSIGNALED(wstatus) || WTERMSIG(wstatus) != SIGTERM)
		throw std::runtime_error("The server reported an error");
}
