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
/*   Updated: 2024/06/07 06:00:27 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEWALLET_HPP
# define EULEEWALLET_HPP

# include "server.hpp"

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
	UPLOAD,
	LIMIT_EXCEPT
};

class EuleeWallet
{
	public:
		typedef Key									key_type;
		typedef std::vector<std::string>			mapped_type;
		typedef std::map<key_type, mapped_type>		value_type;
		typedef std::map<std::string, std::string>	cgi_type;
		typedef	value_type::iterator				iterator;

		EuleeWallet();
		EuleeWallet(value_type location);
		~EuleeWallet();

		mapped_type	&operator[](const key_type &key);

		iterator	begin();
		iterator	end();

		cgi_type	cgi;

	private:
		value_type	_location;
};

#endif