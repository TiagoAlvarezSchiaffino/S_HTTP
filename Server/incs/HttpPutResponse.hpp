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
/*   Updated: 2024/06/06 04:41:50 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPUTRESPONSE_HPP
# define HTTPPUTRESPONSE_HPP

# include "EuleeHand.hpp"

class HttpPutResponse
{
	public:
		HttpPutResponse(EuleeHand *database);
		~HttpPutResponse();
		void		handlePut();

	private:
		EuleeHand	*_database;
};

#endif