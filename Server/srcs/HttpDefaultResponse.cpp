/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpDefaultResponse.cpp                                                  */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:39:19 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 08:17:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpDefaultResponse.hpp"

HttpDefaultResponse::HttpDefaultResponse(int socket) : _socket(socket) {}

HttpDefaultResponse::~HttpDefaultResponse() {}

/* TO BE REMOVED */
int	ft_select(int fd, void *buffer, size_t size, Mode mode)
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

    int ret = select(FD_SETSIZE, &readFds, &writeFds, NULL, &timeout);
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

void	HttpDefaultResponse::handleDefault()
{
	std::string http = "HTTP/1.1 200 OK\n";
	std::string message = "This should display the default page";
	std::string content_length_str = "Content-Length: " + std::to_string(message.length()) + "\n\n";
	std::string output = http + content_length_str + message;

	ft_select(this->_socket, (void *)output.c_str(), output.length(), WRITE);
	close(this->_socket);
}