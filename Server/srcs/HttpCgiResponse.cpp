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
/*   Updated: 2024/06/03 17:34:21 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpCgiResponse.hpp"

HttpCgiResponse::HttpCgiResponse(std::string path, std::string method, int socket, int contentLength) : _path(path), _method(method), _socket(socket), _contentLength(contentLength) {}
HttpCgiResponse::~HttpCgiResponse() {}

/* TO BE REMOVED */
int	ft_select3(int fd, void *buffer, size_t size, Mode mode)
{
	fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    if (mode == READ)
        FD_SET(fd, &read_fds);
    else if (mode == WRITE)
        FD_SET(fd, &write_fds);

    timeval	timeout;
    timeout.tv_sec = WS_TIMEOUT;
    timeout.tv_usec = 0;

    int num_ready = select(fd + 1, &read_fds, &write_fds, nullptr, &timeout);
    if (num_ready == -1)
	{
        std::cerr << "Error: select() failed.\n";
        return (-1);
    }
    else if (num_ready == 0)
	{
        std::cout << "Select timeout.\n";
        return (0);
    }

    if (FD_ISSET(fd, &read_fds) && mode == READ)
        return (read(fd, buffer, size));
    else if (FD_ISSET(fd, &write_fds) && mode == WRITE)
        return (write(fd, buffer, size));
    return (0);
}

void	HttpCgiResponse::handleCgi()
{
	int		cgi_input[2], cgi_output[2], status;
	pid_t	pid;
	char	c;

    if (pipe(cgi_input) < 0 || pipe(cgi_output) < 0)
		this->_perrorExit("Pipe Error");
    if ((pid = fork()) < 0)
		this->_perrorExit("Fork Error");

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
        // setenv("CONTENT_LENGTH", std::to_string(this->_contentLength).c_str(), 1);
		setenv("CONTENT_LENGTH", "69", 1);

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
			int	n = read(this->_socket, &c, 1);
            int i = 0;
            while (n > 0 && i < this->_contentLength) {
                write(cgi_input[1], &c, 1);
				n = ft_select3(this->_socket, &c, 1, READ);
                i++;
            }
        }

		std::string	buffer(WS_BUFFER_SIZE, '\0');
        int n = read(cgi_output[0], &buffer[0], WS_BUFFER_SIZE);
        while (n > 0)
		{
			ft_select3(this->_socket, &buffer[0], n, WRITE);
			n = read(cgi_output[0], &buffer[0], WS_BUFFER_SIZE);
        }

        close(cgi_input[1]);
        close(cgi_output[0]);
        waitpid(pid, &status, 0);
		close(this->_socket);
    }
}