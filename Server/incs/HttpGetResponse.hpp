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
/*   Updated: 2024/06/04 09:12:00 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGETRESPONSE_HPP
# define HTTPGETRESPONSE_HPP

# include "Serv.hpp"

class HttpGetResponse
{
	public:
		HttpGetResponse(std::string path, int socket);
		~HttpGetResponse();
		void		handleGet();

	private:
		int			_socket;
		std::string	_path;
};

#endif