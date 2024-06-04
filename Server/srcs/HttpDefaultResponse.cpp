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
/*   Updated: 2024/06/03 17:38:42 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpDefaultResponse.hpp"

HttpDefaultResponse::HttpDefaultResponse(int socket) : _socket(socket) {}

HttpDefaultResponse::~HttpDefaultResponse() {}

int	ft_select(int fd, void *buffer, size_t size, Mode mode)
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

    int num_ready = select(FD_SETSIZE, &read_fds, &write_fds, NULL, &timeout);
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

	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if (FD_ISSET(fd, &read_fds) && mode == READ && i == fd)
			return (read(fd, buffer, size));
		else if (FD_ISSET(fd, &write_fds) && mode == WRITE && i == fd)
			return (write(fd, buffer, size));
	}
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