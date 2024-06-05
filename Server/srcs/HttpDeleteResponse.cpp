/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpDeleteResponse.cpp                                                   */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 08:46:28 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 14:11:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpDeleteResponse.hpp"

HttpDeleteResponse::HttpDeleteResponse(EuleeHand database) : _database(database) {}

HttpDeleteResponse::~HttpDeleteResponse() {}

void	HttpDeleteResponse::handleDelete()
{
	std::string	filePath = "." + this->_database.methodPath;
	int	result = std::remove(filePath.c_str());
	if (result != 0)
	{
		std::cerr << RED << "Error: " << filePath << " cannot be deleted" << RESET << std::endl;
		this->_database.sendHttp(404);
		return ;
	}
	std::cout << GREEN << filePath << " has been deleted!" << RESET << std::endl;
	close(this->_database.socket);
}