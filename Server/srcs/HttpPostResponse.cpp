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
/*   Updated: 2024/06/04 12:02:45 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpPostResponse.hpp"

HttpPostResponse::HttpPostResponse(int socket, std::string buffer) : _socket(socket), _buffer(buffer) {}

HttpPostResponse::~HttpPostResponse() {}

void	HttpPostResponse::_saveFile(size_t contentLength, int contentLengthSpecified)
{
	size_t		boundaryPos = this->_buffer.find("boundary=") + std::strlen("boundary=");
    if (boundaryPos == std::string::npos)
	{
		std::cerr << RED << "No boundary found!" << RESET << std::endl;
		return ;
	}

	std::string	boundary = this->_buffer.substr(boundaryPos, this->_buffer.find("\r\n", boundaryPos) - boundaryPos);
	boundaryPos = this->_buffer.find(boundary, boundaryPos + boundary.length());
	
	if (contentLengthSpecified && this->_buffer.substr(boundaryPos).length() + std::strlen("\r\n") != contentLength)
	{
		std::cerr << RED << "Error: Content-Length does not match actual content length!" << RESET << std::endl;
		return ;
	}
	size_t	namePos = this->_buffer.find("filename=\"");
	if (namePos == std::string::npos)
	{
		std::cerr << RED << "No file name found!" << RESET << std::endl;
		return ;
	}
	namePos += std::strlen("filename=\"");
	std::string	fileName = this->_buffer.substr(namePos, this->_buffer.find("\"", namePos) - namePos);

	size_t		boundaryEndPos = this->_buffer.find("--" + boundary + "--");
    if (boundaryEndPos == std::string::npos)
	{
		std::cerr << RED << "No end boundary found!" << RESET << std::endl;
		return ;
	}
	size_t		dataLength = boundaryEndPos - (boundaryPos + boundary.length());
	std::string	fileData = this->_buffer.substr(boundaryPos + boundary.length(), dataLength - std::strlen("\r\n"));

	std::ofstream	newFile(fileName, std::ios::binary);
	if (newFile.is_open() == false)
	{
		std::cerr << RED << "Error: Failed to create new file!" << RESET << std::endl;
		return ;
	}
	std::string	toWrite = fileData.substr(fileData.find("\r\n\r\n") + std::strlen("\r\n\r\n"));
	newFile.write(toWrite.c_str(), toWrite.length());
	newFile.close();
}

void	HttpPostResponse::handlePost()
{
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
	ft_select(this->_socket, (void *)response.c_str(), response.length(), WRITE);
    close(this->_socket);
}