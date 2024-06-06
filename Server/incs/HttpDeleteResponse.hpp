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
/*   Updated: 2024/06/06 04:41:17 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETERESPONSE_HPP
# define HTTPDELETERESPONSE_HPP

# include "EuleeHand.hpp"

class HttpDeleteResponse
{
	public:
		HttpDeleteResponse(EuleeHand *database);
		~HttpDeleteResponse();
		void	handleDelete();

	private:
		EuleeHand	*_database;
};

#endif