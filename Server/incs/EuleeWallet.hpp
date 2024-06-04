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
/*   Updated: 2024/06/04 10:18:38 by Tiago                  /_____/ U         */
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
	LOCATION_READ_PATH,
	LISTEN,
	ROOT,
	INDEX,
	SERVER_NAME,
	ERROR_PAGE,
	CLIENT_MAX_BODY_SIZE,
	AUTO_INDEX,
	RETURN,
	CGI,
	INCLUDE,
	UPLOAD,
	LIMIT_EXCEPT
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