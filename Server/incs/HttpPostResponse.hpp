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
/*   Updated: 2024/06/04 11:46:04 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOSTRESPONSE_HPP
# define HTTPPOSTRESPONSE_HPP

# include "Serv.hpp"
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