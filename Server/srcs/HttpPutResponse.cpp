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
/*   Updated: 2024/06/05 09:59:02 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpPutResponse.hpp"

HttpPutResponse::HttpPutResponse(EuleeHand database) : _database(database) {}

HttpPutResponse::~HttpPutResponse() {}

void	HttpPutResponse::handlePut()
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

	std::ofstream	originalPath(this->_database.methodPath.c_str() + 1, std::ios::binary);
	if (originalPath.fail())
	{
		std::cout << RED << "Directory not found, using upload from config..." << RESET << std::endl;
		if (this->_database.server[this->_database.serverIndex].location[this->_database.locationPath][UPLOAD].size() == 0)
			std::cout << RED << "Upload not set in config, cannot save file..." << RESET << std::endl;
		else
		{
			int	pathCanUse = 0;
			for (size_t i = 0; this->_database.server[this->_database.serverIndex].location[this->_database.locationPath][UPLOAD].size() && pathCanUse == 0; i++)
			{
				std::ofstream	locationPath(this->_database.server[this->_database.serverIndex].location[this->_database.locationPath][UPLOAD][i] + this->_database.methodPath.substr(this->_database.methodPath.find_last_of("/")));
				if (locationPath.fail() == false)
				{
					std::cout << GREEN << "Put to: " << this->_database.methodPath.c_str() + 1 << RESET << std::endl;
					std::string		toWrite = this->_database.buffer.substr(this->_database.buffer.find("\r\n\r\n") + std::strlen("\r\n\r\n"));
					if (contentLengthSpecified)
						locationPath.write(toWrite.c_str(), contentLength);
					else
						locationPath.write(toWrite.c_str(), toWrite.length());
					locationPath.close();
					pathCanUse = 1;
				}
			}
			if (pathCanUse == 0)
				std::cout << RED << "Upload path cannot be used to save file..." << RESET << std::endl;
		}
	}
	else
	{
		std::cout << GREEN << "Put to: " << this->_database.methodPath.c_str() + 1 << RESET << std::endl;
		std::string		toWrite = this->_database.buffer.substr(this->_database.buffer.find("\r\n\r\n") + std::strlen("\r\n\r\n"));
		if (contentLengthSpecified)
			originalPath.write(toWrite.c_str(), contentLength);
		else
			originalPath.write(toWrite.c_str(), toWrite.length());
		originalPath.close();
	}
	this->_database.sendHttp(200, 1);
}