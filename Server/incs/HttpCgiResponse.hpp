/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpCgiResponse.hpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 16:58:46 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 07:48:03 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCGIRESPONSE_HPP
# define HTTPCGIRESPONSE_HPP

# include <string>
# include <iostream>
# include <unistd.h>

# define RED			"\033[1;31m"
# define RESET			"\033[0m"
# define WS_BUFFER_SIZE	30000
# define WS_TIMEOUT		3

class HttpCgiResponse
{
	public:
		HttpCgiResponse(std::string path, std::string method, int socket);
		~HttpCgiResponse();
		void		handleCgi();

	private:
		void		_perrorExit(std::string msg);
		std::string	_path, _method;
		int			_socket;
};

#endif