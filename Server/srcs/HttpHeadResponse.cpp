/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpHeadResponse.cpp                                                     */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 09:35:19 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/06 03:31:28 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpHeadResponse.hpp"

HttpHeadResponse::HttpHeadResponse(EuleeHand *database) : _database(database) {}

HttpHeadResponse::~HttpHeadResponse() {}

void	HttpHeadResponse::handleHead()
{
	std::ifstream	file(this->_database->methodPath[this->_database->socket].c_str() + 1);
	std::string		http = "";
	if (file.fail() && this->_database->methodPath[this->_database->socket] != "/")
	{
		std::cerr << RED << "Error opening " << this->_database->methodPath[this->_database->socket] << "!" << RESET << std::endl;
		this->_database->sendHttp(404);
	}
	else
	{
		std::cout << GREEN << "File " << this->_database->methodPath[this->_database->socket] << " found!" << RESET << std::endl;
		this->_database->sendHttp(200);
	}
	file.close();
}