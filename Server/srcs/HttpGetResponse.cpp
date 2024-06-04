/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpGetResponse.cpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 05:55:28 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 06:54:46 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "HttpGetResponse.hpp"

HttpGetResponse::HttpGetResponse(std::string path, int socket) : _socket(socket), _path(path) {}

HttpGetResponse::~HttpGetResponse() {}

/* TO BE REMOVED */
enum	Mode
{
	READ,
	WRITE
};

/* TO BE REMOVED */
int	ft_select2(int fd, void *buffer, size_t size, Mode mode)
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

void	HttpGetResponse::handleGet()
{
	std::ifstream	file(this->_path.c_str() + 1);
	if (file.fail())
	{
		std::cerr << RED << "Error opening " << this->_path << "!\n" << RESET << std::endl;
		std::string responseBody = "404 Not Found";
		std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: " + std::to_string(responseBody.length()) + "\r\n\r\n" + responseBody;
		send(this->_socket, response.c_str(), response.length(), 0);
		close(this->_socket);
		return ;
	}

	file.seekg(0, std::ios::end);
	long	file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string	fileContents;
	fileContents.resize(file_size + 1);
	if (file.read(&fileContents[0], file_size).fail())
	{
		std::cerr << RED << "Error reading " << this->_path << "!\n" << RESET << std::endl;
		file.close();
		close(this->_socket);
		return ;
	}
	file.close();

	std::string	httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: */*\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n" + fileContents;
	int	total = 0;
	while (total < (int)httpResponse.size())
	{
		int sent = ft_select2(this->_socket, &httpResponse[total], httpResponse.size() - total, WRITE);
		if (sent <= 0)
		{
			close(this->_socket);
			return ;
		}
		total += sent;
		std::cout << "Sent: " << sent << "\tTotal: " << total << " / " << httpResponse.size() << std::endl;
	}
	close(this->_socket);
}