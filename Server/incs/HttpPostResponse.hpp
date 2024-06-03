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
/*   Updated: 2024/06/03 14:34:59 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOSTRESPONSE_HPP
# define HTTPPOSTRESPONSE_HPP

# include <iostream>
# include <string>
# include <unistd.h>
# include <sys/socket.h>

class HttpPostResponse
{
	public:
		HttpPostResponse(int socket, int content_length, int valread, std::string buffer);
		~HttpPostResponse();
		void	handlePost();

	private:
		int			_socket, _content_length, _valread;
		std::string	_buffer;
};

#endif