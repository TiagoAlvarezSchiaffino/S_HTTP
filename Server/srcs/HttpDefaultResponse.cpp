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
/*   Updated: 2024/06/04 14:11:06 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpDefaultResponse.hpp"

/* MIGHT NOT NEED ANYMORE */
HttpDefaultResponse::HttpDefaultResponse(EuleeHand database) : _database(database) {}

HttpDefaultResponse::~HttpDefaultResponse() {}

void	HttpDefaultResponse::handleDefault()
{
	std::string http = "HTTP/1.1 200 OK\r\n";
	std::string message = "This should display the default page";
	std::string content_length_str = "Content-Length: " + std::to_string(message.length()) + "\r\n\r\n";
	std::string output = http + content_length_str + message;

	this->_database.buffer[this->_database.socket] = output;
}