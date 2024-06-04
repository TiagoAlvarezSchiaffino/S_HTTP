/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpPostResponse.cpp                                                     */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 17:38:42 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 18:30:05 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpPostResponse.hpp"

HttpPostResponse::HttpPostResponse(int socket, int content_length, int valread, std::string buffer) : _socket(socket), _content_length(content_length), _valread(valread), _buffer(buffer) {}
HttpPostResponse::~HttpPostResponse() {}

/* TO BE REMOVED */
enum	Mode
{
	READ,
	WRITE
};

/* TO BE REMOVED */
int	ft_select1(int fd, void *buffer, size_t size, Mode mode)
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

void	HttpPostResponse::handlePost()
{
	size_t	content_length_pos = this->_buffer.find("Content-Length: ");
	if (content_length_pos != std::string::npos)
	{
		content_length_pos += std::strlen("Content-Length: ");
		this->_content_length = std::stoi(this->_buffer.substr(content_length_pos));
	}

	std::string	message_body;
	message_body.resize(this->_content_length);
	this->_valread = ft_select1(this->_socket, &message_body[0], this->_content_length, READ);


	std::string response_body = "Server has received your POST request!";
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response_body.length()) + "\r\n\r\n" + response_body;
	send(this->_socket, response.c_str(), response.length(), 0);
}