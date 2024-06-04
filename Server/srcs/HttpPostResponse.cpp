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
/*   Updated: 2024/06/04 06:56:28 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpPostResponse.hpp"

HttpPostResponse::HttpPostResponse(int socket, int content_length, int valread, std::string buffer) : _socket(socket), _contentLength(content_length), _valread(valread), _buffer(buffer) {}

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

	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if (FD_ISSET(fd, &readFds) && mode == READ && i == fd)
			return (read(fd, buffer, size));
		else if (FD_ISSET(fd, &writeFds) && mode == WRITE && i == fd)
			return (write(fd, buffer, size));
	}
    return (0);
}

void	HttpPostResponse::_saveFile()
{
	size_t	namePos = this->_messageBody.find("filename=\"");
	std::string	fileName, contentType;
	if (namePos != std::string::npos)
	{
		namePos += std::strlen("filename=\"");
		fileName = this->_messageBody.substr(namePos, this->_messageBody.find("\"", namePos) - namePos);
	}

	size_t		boundaryPos = this->_messageBody.find("------WebKitFormBoundary");
	std::string	boundary = this->_messageBody.substr(boundaryPos, this->_messageBody.find("\r\n", boundaryPos) - boundaryPos);

	size_t		boundaryEndPos = this->_messageBody.find("\r\n" + boundary + "--");

	size_t		dataLength = boundaryEndPos - (boundaryPos + boundary.length());
	std::string	fileData = this->_messageBody.substr(boundaryPos + boundary.length(), dataLength);

	std::ofstream	newFile(fileName, std::ios::binary);
	if (newFile.is_open() == false)
	{
		std::cerr << RED << "Error: Failed to create new file." << RESET << std::endl;
		return ;
	}
	std::string	toWrite = fileData.substr(fileData.find("\r\n\r\n") + 4);
	
    newFile.write(toWrite.c_str(), toWrite.length());
	newFile.close();
}

void	HttpPostResponse::handlePost()
{
	size_t	contentLengthPos = this->_buffer.find("Content-Length: ");
	if (contentLengthPos != std::string::npos)
	{
		contentLengthPos += std::strlen("Content-Length: ");
		this->_contentLength = std::stoi(this->_buffer.substr(contentLengthPos));
	}

	this->_messageBody.resize(this->_contentLength);
	int	total = 0;
	while (total < this->_contentLength)
	{
		this->_valread = ft_select1(this->_socket, &this->_messageBody[total], this->_contentLength - total, READ);
		if (this->_valread <= 0)
		{
			close(this->_socket);
			return ;
		}
		total += this->_valread;
		std::cout << "Received: " << this->_valread << "\tTotal: " << total << " / " << this->_contentLength << std::endl;
	}

	this->_saveFile();
	std::string responseBody = "Server has received your POST request!";
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseBody.length()) + "\r\n\r\n" + responseBody;
	send(this->_socket, response.c_str(), response.length(), 0);
}