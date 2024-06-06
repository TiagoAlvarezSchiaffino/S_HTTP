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
/*   Updated: 2024/06/06 04:41:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADRESPONE_HPP
# define HTTPHEADRESPONE_HPP

# include "EuleeHand.hpp"

class HttpHeadResponse
{
	public:
		HttpHeadResponse(EuleeHand *database);
		~HttpHeadResponse();
		void	handleHead();

	private:
		EuleeHand	*_database;
};

#endif