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
/*   Updated: 2024/06/04 09:10:17 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCGIRESPONSE_HPP
# define HTTPCGIRESPONSE_HPP

# include "Serv.hpp"

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