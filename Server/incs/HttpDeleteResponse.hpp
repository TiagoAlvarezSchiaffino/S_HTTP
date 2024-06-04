/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpDeleteResponse.hpp                                                   */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 08:40:21 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 09:11:44 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETERESPONSE_HPP
# define HTTPDELETERESPONSE_HPP

# include "Serv.hpp"

class HttpDeleteResponse
{
	public:
		HttpDeleteResponse(int socket, std::string path);
		~HttpDeleteResponse();
		void	handleDelete();

	private:
		int		_socket;
		std::string	_path;
};

#endif