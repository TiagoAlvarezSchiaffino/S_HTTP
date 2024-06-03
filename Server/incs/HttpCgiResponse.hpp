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
/*   Updated: 2024/06/03 17:01:03 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCGIRESPONSE_HPP
# define HTTPCGIRESPONSE_HPP

# include <string>
# include <iostream>
# include <unistd.h>
# include <poll.h>

# define RED			"\033[1;31m"
# define RESET			"\033[0m"
# define WS_BUFFER_SIZE	30000
# define WS_TIMEOUT		3000

class HttpCgiResponse
{
	public:
		HttpCgiResponse(pollfd (&fds)[1], std::string path, std::string method, int socket, int contentLength);
		~HttpCgiResponse();
		void	handleCgi();

	private:
		void		_perrorExit(std::string msg);
		std::string	_path, _method;
		int			_socket, _contentLength;
		pollfd		(&_fds)[1];
};

#endif