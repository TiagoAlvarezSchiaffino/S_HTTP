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
/*   Updated: 2024/06/04 15:11:46 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpGetResponse.hpp"

HttpGetResponse::HttpGetResponse(EuleeHand *database) : _database(database) {}

HttpGetResponse::~HttpGetResponse() {}

void	HttpGetResponse::handleGet()
{
	size_t	queryPos = this->_database->methodPath.find('?');
	if (queryPos != std::string::npos)
		this->_database->methodPath = this->_database->methodPath.substr(0, queryPos);

	if (this->_database->checkPath(this->_database->methodPath.c_str() + 1, 0, 0))
	{
		std::cerr << RED << "Error opening " << this->_database->methodPath << "!\n" << RESET << std::endl;
		this->_database->sendHttp(404);
		return ;
	}

	std::ifstream	file(this->_database->methodPath.c_str() + 1);
	file.seekg(0, std::ios::end);
	long	file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string	fileContents;
	fileContents.resize(file_size + 1);

	if (file.read(&fileContents[0], file_size).fail())
	{
		std::cerr << RED << "Error reading " << this->_database->methodPath << "!" << RESET << std::endl;
		if (this->_database->useDirectoryListing)
		{
			this->_database->buffer[this->_database->socket] = "HTTP/1.1 200 OK\r\n\r\n" + this->_database->directoryListing(this->_database->methodPath);
			std::cout << GREEN << "Autoindex is set on, directory listing sent!" << RESET << std::endl;
		}
		else if (this->_database->checkPath(this->_database->methodPath.c_str() + 1, 0, 1) || this->_database->useDefaultIndex)
			this->_database->sendHttp(200);
		else
			this->_database->sendHttp(404);
		file.close();
		return ;
	}
	file.close();

	std::cout << "Should be here" << std::endl;
	this->_database->response[this->_database->socket] = "HTTP/1.1 200 OK\r\nContent-Type: */*\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n" + fileContents;
}