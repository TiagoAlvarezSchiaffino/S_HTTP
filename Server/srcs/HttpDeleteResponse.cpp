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
/*   Updated: 2024/06/04 18:58:52 by Tiago                  /_____/ U         */
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
		this->_database.sendHttp(200, this->_database.methodPath.c_str());

        std::string	response = "HTTP/1.1 404 Not Found\r\n\r\nFile to delete is not found...\r\n";
		this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.length(), WRITE);
		close(this->_database.socket);
		return ;
	}
	std::cout << GREEN << filePath << " has been deleted!" << RESET << std::endl;
	close(this->_database.socket);
}