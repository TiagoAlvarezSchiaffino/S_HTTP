/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpPutResponse.cpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 12:02:45 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 12:10:06 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpPutResponse.hpp"

HttpPutResponse::HttpPutResponse(int socket, std::string buffer, std::string path, EuleeHand database) : _socket(socket), _buffer(buffer), _path(path), _database(database) {}

HttpPutResponse::~HttpPutResponse() {}

void	HttpPutResponse::_saveFile(size_t contentLength, int contentLengthSpecified)
{
	// std::cout << (this->_path.c_str() + 1) << std::endl;
	std::ofstream	newFile(this->_path.c_str() + 1, std::ios::binary);
	return ;
	(void)contentLength;
	(void)contentLengthSpecified;
}

void	HttpPutResponse::handlePut()
{
	std::cout << "Should put file here!" << std::endl;
	int		contentLengthSpecified = 0;
	size_t	contentLength = 0;
	size_t	contentLengthPos = this->_buffer.find("Content-Length: ");
	if (contentLengthPos != std::string::npos)
	{
		contentLengthPos += std::strlen("Content-Length: ");
		contentLength = std::stoul(this->_buffer.substr(contentLengthPos));
		contentLengthSpecified = 1;
	}

	this->_saveFile(contentLength, contentLengthSpecified);
	std::string responseBody = "Server has received your POST request!";
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseBody.length()) + "\r\n\r\n" + responseBody;
	this->_database.ft_select(this->_socket, (void *)response.c_str(), response.length(), WRITE);
	close(this->_socket);
}