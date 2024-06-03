/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpCgiResponse.cpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 17:03:30 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 17:21:55 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpCgiResponse.hpp"

HttpCgiResponse::HttpCgiResponse(pollfd (&fds)[1], std::string path, std::string method, int socket, int contentLength) : _path(path), _method(method), _socket(socket), _contentLength(contentLength), _fds(fds) {}

HttpCgiResponse::~HttpCgiResponse() {}

/* TO BE REMOVED */
void	HttpCgiResponse::_perrorExit(std::string msg)
{
	std::cerr << RED << msg << ": ";
	perror("");
	std::cerr << RESET;
	exit(EXIT_FAILURE);
}

/* TO BE REMOVED */
enum	Mode
{
	READ,
	WRITE
};

/* TO BE REMOVED */
int	ft_poll(pollfd (&fds)[1], int fd, void *buffer, size_t size, Mode mode)
{
	int	ret;

	ret = poll(fds, 1, WS_TIMEOUT);
	if (ret == -1)
		std::cout << RED << "Poll error" << RESET << std::endl;
	else if (ret == 0)
		std::cout << RED << "Poll timeout" << RESET << std::endl;
	if (ret <= 0)
		return (-1);

	if (fds[0].revents & POLLIN && mode == READ)
		return (read(fd, buffer, size));
	else if (fds[0].revents & POLLOUT && mode == WRITE)
		return (write(fd, buffer, size));
	return (0);
}

void	HttpCgiResponse::handleCgi()
{
	int		cgi_input[2], cgi_output[2], status;
	pid_t	pid;
	char	c;

    if (pipe(cgi_input) < 0 || pipe(cgi_output) < 0)
		this->_perrorExit("pipe failed");
    if ((pid = fork()) < 0)
		this->_perrorExit("fork failed");

    if (pid == 0)	// child process
	{
        close(cgi_input[1]);
        close(cgi_output[0]);

        dup2(cgi_input[0], STDIN_FILENO);
        dup2(cgi_output[1], STDOUT_FILENO);

		// setenv("REQUEST_METHOD", method, 1);
        // setenv("SCRIPT_NAME", path, 1);
        // setenv("QUERY_STRING", query_string, 1);
        // setenv("CONTENT_TYPE", content_type, 1);
        setenv("CONTENT_LENGTH", std::to_string(this->_contentLength).c_str(), 1);

		char	*cmds[2] = {(char *)(this->_path.c_str() + 1), NULL};
		execve(cmds[0], cmds, NULL);
		std::cerr << RED << "Failed to execve CGI: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
	else	// parent process
	{
        close(cgi_input[0]);
        close(cgi_output[1]);

        if (this->_method == "POST")
		{
			int	n = ft_poll(this->_fds, this->_socket, &c, 1, READ);
            int i = 0;
            while (n > 0 && i < this->_contentLength) {
                write(cgi_input[1], &c, 1);
                n = ft_poll(this->_fds, this->_socket, &c, 1, READ);
                i++;
            }
        }

		std::string	buffer(WS_BUFFER_SIZE, '\0');
        int n = read(cgi_output[0], &buffer[0], WS_BUFFER_SIZE);
        while (n > 0)
		{
			ft_poll(this->_fds, this->_socket, &buffer[0], n, WRITE);
			n = read(cgi_output[0], &buffer[0], WS_BUFFER_SIZE);
        }

        close(cgi_input[1]);
        close(cgi_output[0]);
        waitpid(pid, &status, 0);
		close(this->_socket);
    }
}