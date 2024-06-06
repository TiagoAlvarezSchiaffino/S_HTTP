/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpPostResponse.hpp                                                     */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:33:13 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/06 04:41:42 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOSTRESPONSE_HPP
# define HTTPPOSTRESPONSE_HPP

# include "EuleeHand.hpp"

class HttpPostResponse
{
	public:
		HttpPostResponse(EuleeHand *database);
		~HttpPostResponse();
		void		handlePost();

	private:
		void		_normalSave();
		void		_saveFile();

		EuleeHand	*_database;
		size_t		_contentLength;
		int 		_contentLengthSpecified;
};

#endif