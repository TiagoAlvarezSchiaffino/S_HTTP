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
/*   Updated: 2024/06/04 09:35:19 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpGetResponse.hpp"

HttpGetResponse::HttpGetResponse(std::string path, int socket) : _socket(socket), _path(path) {}

HttpGetResponse::~HttpGetResponse() {}

void	HttpGetResponse::handleGet()
{
    std::string	failedResponse = "HTTP/1.1 404 Not Found\r\n\r\nFile to get is not found...\r\n";
    size_t	queryPos = this->_path.find('?');
	if (queryPos != std::string::npos)
		this->_path = this->_path.substr(0, queryPos);

	std::ifstream	file(this->_path.c_str() + 1);
	if (file.fail())
	{
		std::cerr << RED << "Error opening " << this->_path << "!\n" << RESET << std::endl;
		ft_select(this->_socket, (void *)failedResponse.c_str(), failedResponse.length(), WRITE);
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
		ft_select(this->_socket, (void *)failedResponse.c_str(), failedResponse.length(), WRITE);
        file.close();
		close(this->_socket);
		return ;
	}
	file.close();

	std::string	httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: */*\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n" + fileContents;
	int	total = 0;
	while (total < (int)httpResponse.size())
	{
		int sent = ft_select(this->_socket, &httpResponse[total], httpResponse.size() - total, WRITE);
		if (sent <= 0)
		{
			close(this->_socket);
			return ;
		}
		total += sent;
		std::cout << GREEN << "Sent: " << sent << "\tTotal: " << total << " / " << httpResponse.size() << RESET << std::endl;
	}
	close(this->_socket);
}