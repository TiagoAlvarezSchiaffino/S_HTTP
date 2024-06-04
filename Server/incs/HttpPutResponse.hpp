/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpPutResponse.hpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 11:46:04 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 12:31:09 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPUTRESPONSE_HPP
# define HTTPPUTRESPONSE_HPP

# include "Serv.hpp"

class HttpPutResponse
{
	public:
		HttpPutResponse(int socket, std::string buffer, std::string path, EuleeHand database);
		~HttpPutResponse();
		void		handlePut();

	private:
		int			_socket;
		std::string	_buffer, _path;
		EuleeHand	_database;
};

#endif