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
/*   Updated: 2024/06/04 09:46:37 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpHeadResponse.hpp"

HttpHeadResponse::HttpHeadResponse(int socket, std::string path) : _socket(socket), _path(path) {}

HttpHeadResponse::~HttpHeadResponse() {}

void	HttpHeadResponse::handleHead()
{
	std::ifstream	file(this->_path.c_str() + 1);
	std::string		http = "";
	if (file.fail() && this->_path != "/")
	{
		std::cerr << RED << "Error opening " << this->_path << "!\n" << RESET << std::endl;
		http = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n";
	}
	else
	{
		std::cout << GREEN << "File " << this->_path << " found!!\n" << RESET << std::endl;
		http = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n";
	}
	ft_select(this->_socket, (void *)http.c_str(), http.length(), WRITE);
	file.close();
	close(this->_socket);
}