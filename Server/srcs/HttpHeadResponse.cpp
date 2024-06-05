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
/*   Updated: 2024/06/04 14:34:16 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpHeadResponse.hpp"

HttpHeadResponse::HttpHeadResponse(EuleeHand database) : _database(database) {}

HttpHeadResponse::~HttpHeadResponse() {}

void	HttpHeadResponse::handleHead()
{
	std::ifstream	file(this->_database.methodPath.c_str() + 1);
	std::string		http = "";
	if (file.fail() && this->_database.methodPath != "/")
	{
		std::cerr << RED << "Error opening " << this->_database.methodPath << "!" << RESET << std::endl;
		this->_database.sendHttp(404);
	}
	else
	{
		std::cout << GREEN << "File " << this->_database.methodPath << " found!" << RESET << std::endl;
		this->_database.sendHttp(200);
	}
	file.close();
}