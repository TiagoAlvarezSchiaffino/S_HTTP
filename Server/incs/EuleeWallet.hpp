/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeWallet.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 17:59:56 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/05 10:26:35 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEWALLET_HPP
# define EULEEWALLET_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

enum	Key
{
	LOCATION_READ_PATH, //USING
	LISTEN, //USING
	ROOT, //USING
	INDEX, //USING
	SERVER_NAME, //USING
	ERROR_PAGE,
	CLIENT_MAX_BODY_SIZE,
	AUTO_INDEX,
	RETURN, //TBR
	CGI,
	INCLUDE,
	UPLOAD, //USING
	LIMIT_EXCEPT //USING
};

// location block
class EuleeWallet
{
	public:
		typedef Key								key_type;
		typedef std::vector<std::string>		mapped_type;
		typedef std::map<key_type, mapped_type>	value_type;
		typedef	value_type::iterator			iterator;

		EuleeWallet(void);
		EuleeWallet(value_type location);
		~EuleeWallet(void);

		mapped_type	&operator[](const key_type &key);

		iterator	begin(void);
		iterator	end(void);

	private:
		value_type	_location;
};

#endif