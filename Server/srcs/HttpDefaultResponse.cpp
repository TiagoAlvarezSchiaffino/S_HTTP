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
/*   Updated: 2024/06/03 15:47:33 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpDefaultResponse.hpp"

HttpDefaultResponse::HttpDefaultResponse(pollfd (&fds)[1], int socket) : _fds(fds), _socket(socket) {}

HttpDefaultResponse::~HttpDefaultResponse() {}

void	HttpDefaultResponse::handleDefault()
{
	std::string http = "HTTP/1.1 200 OK\n";
	std::string message = "This should display the default page";
	std::string content_length_str = "Content-Length: " + std::to_string(message.length()) + "\n\n";
	std::string output = http + content_length_str + message;

	int	ret = poll(this->_fds, 1, WS_TIMEOUT);
	if (ret == -1)
		std::cout << RED << "Poll error" << RESET << std::endl;
	else if (ret == 0)
		std::cout << RED << "Poll timeout" << RESET << std::endl;
	else if (this->_fds[0].revents & POLLOUT)
	{
		write(this->_socket, output.c_str(), output.length());
		std::cout << "Default page sent" << std::endl;
	}
	close(this->_socket);
}