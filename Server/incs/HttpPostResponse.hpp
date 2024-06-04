/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpPostResponse.hpp                                                     */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:33:13 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 07:48:16 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOSTRESPONSE_HPP
# define HTTPPOSTRESPONSE_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <unistd.h>
# include <sys/socket.h>

# define RED			"\033[1;31m"
# define RESET			"\033[0m"
# define WS_BUFFER_SIZE	30000
# define WS_TIMEOUT		3
class HttpPostResponse
{
	public:
		HttpPostResponse(int socket, std::string buffer);
		~HttpPostResponse();
		void		handlePost();

	private:
		void		_saveFile(size_t contentLength, int contentLengthSpecified);
		int			_socket;
		std::string	_buffer;
};

#endif