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
/*   Updated: 2024/06/04 19:05:31 by Tiago                  /_____/ U         */
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
		std::cerr << RED << "Error opening " << this->_database.methodPath << "!\n" << RESET << std::endl;
		this->_database.sendHttp(404, this->_database.methodPath);

        http = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n";
	}
	else
	{
		std::cout << GREEN << "File " << this->_database.methodPath << " found!!\n" << RESET << std::endl;
		http = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n";
	}
	this->_database.ft_select(this->_database.socket, (void *)http.c_str(), http.length(), WRITE);
	file.close();
	close(this->_database.socket);
}