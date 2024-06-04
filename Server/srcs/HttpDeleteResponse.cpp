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
/*   Updated: 2024/06/04 09:31:50 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpDeleteResponse.hpp"

HttpDeleteResponse::HttpDeleteResponse(int socket, std::string path) : _socket(socket), _path(path) {}

HttpDeleteResponse::~HttpDeleteResponse() {}

void	HttpDeleteResponse::handleDelete()
{
	std::string	filePath = "." + this->_path;
	int	result = std::remove(filePath.c_str());
	if (result != 0)
	{
		std::cerr << RED << "Error: " << filePath << " cannot be deleted" << RESET << std::endl;
		std::string	response = "HTTP/1.1 404 Not Found\r\n\r\nFile to delete is not found...\r\n";
		ft_select(this->_socket, (void *)response.c_str(), response.length(), WRITE);
        close(this->_socket);
		return ;
	}
	std::cout << GREEN << filePath << " has been deleted!" << RESET << std::endl;
	close(this->_socket);
}