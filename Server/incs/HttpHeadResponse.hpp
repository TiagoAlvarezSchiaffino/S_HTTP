/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpHeadResponse.hpp                                                     */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 09:12:00 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 09:14:51 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADRESPONE_HPP
# define HTTPHEADRESPONE_HPP

# include "Serv.hpp"

class HttpHeadResponse
{
	public:
		HttpHeadResponse(int socket, std::string path);
		~HttpHeadResponse();
		void	handleHead();

	private:
		int		_socket;
		std::string	_path;
};

#endif