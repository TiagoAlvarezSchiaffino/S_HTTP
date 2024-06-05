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
/*   Updated: 2024/06/05 10:30:01 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDEFAULTRESPONSE_HPP
# define HTTPDEFAULTRESPONSE_HPP

# include "Serv.hpp"

/* MIGHT NOT NEED ANYMORE */
class HttpDefaultResponse
{
	public:
		HttpDefaultResponse(EuleeHand database);
		~HttpDefaultResponse();
		void	handleDefault();

	private:
		EuleeHand	_database;
};

#endif