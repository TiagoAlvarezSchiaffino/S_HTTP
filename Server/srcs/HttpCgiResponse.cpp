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
/*   Updated: 2024/06/04 14:10:01 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpCgiResponse.hpp"

HttpCgiResponse::HttpCgiResponse(EuleeHand database) : _database(database) {}

HttpCgiResponse::~HttpCgiResponse() {}

void	HttpCgiResponse::handleCgi()
{
	int		cgiInput[2], cgiOutput[2], status;
	pid_t	pid;

    if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0)
		perrorExit("Pipe Error");
    if ((pid = fork()) < 0)
		perrorExit("Fork Error");

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

		char	*cmds[2] = {(char *)(this->_database.methodPath.c_str() + 1), NULL};
		execve(cmds[0], cmds, NULL);
		std::cerr << RED << "Failed to execve CGI: " << strerror(errno) << RESET << std::endl;
        std::cout << "HTTP/1.1 404 Not Found\r\n\r\nCGI requested is not found...\r\n" << std::endl;
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
			this->_database.ft_select(this->_database.socket, &buffer[0], n, WRITE);
			n = read(cgiOutput[0], &buffer[0], WS_BUFFER_SIZE);
        }

        close(cgiInput[1]);
        close(cgiOutput[0]);
        waitpid(pid, &status, 0);
		close(this->_database.socket);
    }
}