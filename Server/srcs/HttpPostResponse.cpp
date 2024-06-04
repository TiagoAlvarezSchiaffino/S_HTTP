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
/*   Updated: 2024/06/04 14:55:22 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpPostResponse.hpp"

HttpPostResponse::HttpPostResponse(EuleeHand database) : _database(database) {}

HttpPostResponse::~HttpPostResponse() {}

int	HttpPostResponse::_saveFile(size_t contentLength, int contentLengthSpecified)
{
	size_t		boundaryPos = this->_database.buffer.find("boundary=") + std::strlen("boundary=");
	if (boundaryPos == std::string::npos)
	{
		std::cerr << RED << "No boundary found!" << RESET << std::endl;
		return (0);
	}

	std::string	boundary = this->_database.buffer.substr(boundaryPos, this->_database.buffer.find("\r\n", boundaryPos) - boundaryPos);
	boundaryPos = this->_database.buffer.find(boundary, boundaryPos + boundary.length());
	
	if (contentLengthSpecified && this->_database.buffer.substr(boundaryPos).length() + std::strlen("\r\n") != contentLength)
	{
		std::cerr << RED << "Error: Content-Length does not match actual content length!" << RESET << std::endl;
		return (0);
	}
	size_t	namePos = this->_database.buffer.find("filename=\"");
	if (namePos == std::string::npos)
	{
		std::cerr << RED << "No file name found!" << RESET << std::endl;
		return (0);
	}
	namePos += std::strlen("filename=\"");
	std::string	fileName = this->_database.buffer.substr(namePos, this->_database.buffer.find("\"", namePos) - namePos);

	size_t		boundaryEndPos = this->_database.buffer.find("--" + boundary + "--");
	if (boundaryEndPos == std::string::npos)
	{
		std::cerr << RED << "No end boundary found!" << RESET << std::endl;
		return (0);
	}
	size_t		dataLength = boundaryEndPos - (boundaryPos + boundary.length());
	std::string	fileData = this->_database.buffer.substr(boundaryPos + boundary.length(), dataLength - std::strlen("\r\n"));

	std::ofstream	newFile(fileName, std::ios::binary);
	if (newFile.is_open() == false)
	{
		std::cerr << RED << "Error: Failed to create new file!" << RESET << std::endl;
		return (0);
	}
	std::string	toWrite = fileData.substr(fileData.find("\r\n\r\n") + std::strlen("\r\n\r\n"));
	newFile.write(toWrite.c_str(), toWrite.length());
	newFile.close();
	return (1);
}

void	HttpPostResponse::handlePost()
{
	int		contentLengthSpecified = 0;
	size_t	contentLength = 0;
	size_t	contentLengthPos = this->_database.buffer.find("Content-Length: ");
	if (contentLengthPos != std::string::npos)
	{
		contentLengthPos += std::strlen("Content-Length: ");
		contentLength = std::stoul(this->_database.buffer.substr(contentLengthPos));
		contentLengthSpecified = 1;
	}

	this->_saveFile(contentLength, contentLengthSpecified);
	std::string responseBody = "Server has received your POST request!";
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseBody.length()) + "\r\n\r\n" + responseBody;
	this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.length(), WRITE);

	close(this->_database.socket);
}