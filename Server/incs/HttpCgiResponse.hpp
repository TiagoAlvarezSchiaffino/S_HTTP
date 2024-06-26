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
/*   Updated: 2024/06/06 04:41:10 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCGIRESPONSE_HPP
# define HTTPCGIRESPONSE_HPP

# include "EuleeHand.hpp"

class HttpCgiResponse
{
	public:
		HttpCgiResponse(EuleeHand *database);
		~HttpCgiResponse();
		void		handleCgi();

	private:
		EuleeHand	*_database;
};

#endif