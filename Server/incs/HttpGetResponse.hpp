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
/*   Updated: 2024/06/06 04:41:24 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGETRESPONSE_HPP
# define HTTPGETRESPONSE_HPP

# include "EuleeHand.hpp"

class HttpGetResponse
{
	public:
		HttpGetResponse(EuleeHand *database);
		~HttpGetResponse();
		void		handleGet();

	private:
		EuleeHand	*_database;
};

#endif