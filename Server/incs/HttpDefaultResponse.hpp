/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpDefaultResponse.hpp                                                  */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:30:58 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 17:28:14 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDEFAULTRESPONSE_HPP
# define HTTPDEFAULTRESPONSE_HPP

# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/select.h>

# include "serv.hpp"

class HttpDefaultResponse
{
	public:
		HttpDefaultResponse(int socket);
		~HttpDefaultResponse();
		void	handleDefault();

	private:
		int		_socket;
};

#endif