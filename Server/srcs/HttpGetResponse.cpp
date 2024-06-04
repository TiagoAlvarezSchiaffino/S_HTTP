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
/*   Updated: 2024/06/04 06:11:17 by Tiago                  /_____/ U         */
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

void	HttpGetResponse::handleGet()
{
	std::ifstream	file(this->_path.c_str() + 1);
	if (file.fail())
	{
		std::cerr << RED << "Error opening " << this->_path << "!\n" << RESET << std::endl;
		std::string response_body = "404 Not Found";
		std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: " + std::to_string(response_body.length()) + "\r\n\r\n" + response_body;
		send(this->_socket, response.c_str(), response.length(), 0);
		close(this->_socket);
		return ;
	}

	file.seekg(0, std::ios::end);
	long	file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string	file_contents;
	file_contents.resize(file_size + 1);
	if (file.read(&file_contents[0], file_size).fail())
	{
		std::cerr << RED << "Error reading " << this->_path << "!\n" << RESET << std::endl;
		file.close();
		close(this->_socket);
		return ;
	}
	file.close();

	std::string	http_response = "HTTP/1.1 200 OK\r\nContent-Type: */*\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n" + file_contents;
	int	total = 0;
	while (total < (int)http_response.size())
	{
		int sent = ft_select2(this->_socket, &http_response[total], http_response.size() - total, WRITE);
		if (sent <= 0)
		{
			close(this->_socket);
			return ;
		}
		total += sent;
		std::cout << "Sent: " << sent << "\tTotal: " << total << " / " << http_response.size() << std::endl;
	}
	close(this->_socket);
}