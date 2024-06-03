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
/*   Updated: 2024/06/03 14:33:05 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDEFAULTRESPONSE_HPP
# define HTTPDEFAULTRESPONSE_HPP

# include <string>
# include <iostream>
# include <unistd.h>

class HttpDefaultResponse
{
	public:
		HttpDefaultResponse(int socket);
		~HttpDefaultResponse();
		void	handleDefault();

	private:
		int	_socket;
};

#endif