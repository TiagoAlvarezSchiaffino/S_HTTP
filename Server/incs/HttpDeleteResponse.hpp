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
/*   Updated: 2024/06/04 13:45:16 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETERESPONSE_HPP
# define HTTPDELETERESPONSE_HPP

# include "Serv.hpp"

class HttpDeleteResponse
{
	public:
		HttpDeleteResponse(EuleeHand database);
		~HttpDeleteResponse();
		void	handleDelete();

	private:
		EuleeHand	_database;
};

#endif