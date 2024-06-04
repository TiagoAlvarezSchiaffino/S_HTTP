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
/*   Updated: 2024/06/04 07:06:40 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpCgiResponse.hpp"

HttpCgiResponse::HttpCgiResponse(std::string path, std::string method, int socket) : _path(path), _method(method), _socket(socket) {}

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
int	ft_select3(int fd, void *buffer, size_t size, Mode mode)
{
	fd_set readFds, writeFds;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    if (mode == READ)
        FD_SET(fd, &readFds);
    else if (mode == WRITE)
        FD_SET(fd, &writeFds);

    timeval	timeout;
    timeout.tv_sec = WS_TIMEOUT;
    timeout.tv_usec = 0;

    int ret = select(fd + 1, &readFds, &writeFds, nullptr, &timeout);
    if (ret == -1)
	{
        std::cerr << "Error: select() failed.\n";
        return (-1);
    }
    else if (ret == 0)
	{
        std::cout << "Select timeout.\n";
        return (0);
    }

    if (FD_ISSET(fd, &readFds) && mode == READ)
        return (read(fd, buffer, size));
    else if (FD_ISSET(fd, &writeFds) && mode == WRITE)
        return (write(fd, buffer, size));
    return (0);
}

void	HttpCgiResponse::handleCgi()
{
	int		cgiInput[2], cgiOutput[2], status;
	pid_t	pid;

    if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0)
		this->_perrorExit("Pipe Error");
    if ((pid = fork()) < 0)
		this->_perrorExit("Fork Error");

    if (pid == 0)	// child process
	{
        close(cgiInput[1]);
        close(cgiOutput[0]);

        dup2(cgiInput[0], STDIN_FILENO);
        dup2(cgiOutput[1], STDOUT_FILENO);

		// setenv("REQUEST_METHOD", method, 1);
        // setenv("SCRIPT_NAME", path, 1);
        // setenv("QUERY_STRING", query_string, 1);
        // setenv("CONTENT_TYPE", content_type, 1);
        // setenv("CONTENT_LENGTH", std::to_string(contentLength.c_str(), 1);
		setenv("CONTENT_LENGTH", "69", 1);

		char	*cmds[2] = {(char *)(this->_path.c_str() + 1), NULL};
		execve(cmds[0], cmds, NULL);
		std::cerr << RED << "Failed to execve CGI: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
	else	// parent process
	{
        close(cgiInput[0]);
        close(cgiOutput[1]);

		std::string	buffer(WS_BUFFER_SIZE, '\0');
        int n = read(cgiOutput[0], &buffer[0], WS_BUFFER_SIZE);
        while (n > 0)
		{
			ft_select3(this->_socket, &buffer[0], n, WRITE);
			n = read(cgiOutput[0], &buffer[0], WS_BUFFER_SIZE);
        }

        close(cgiInput[1]);
        close(cgiOutput[0]);
        waitpid(pid, &status, 0);
		close(this->_socket);
    }
}