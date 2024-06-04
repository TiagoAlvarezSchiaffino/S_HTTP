/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpGetResponse.hpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 05:51:50 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 05:54:44 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGETRESPONSE_HPP
# define HTTPGETRESPONSE_HPP

# include <fstream>
# include <iostream>
# include <string>
# include <unistd.h>
# include <sys/socket.h>

# define RED		"\033[1;31m"
# define RESET		"\033[0m"
# define WS_TIMEOUT	3

class HttpGetResponse
{
	public:
		HttpGetResponse(std::string path, int socket);
		~HttpGetResponse();
		void	handleGet();

	private:
		int			_socket;
		std::string	_path;
};

#endif