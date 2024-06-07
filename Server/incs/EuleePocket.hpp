/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleePocket.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 16:53:59 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/07 05:59:51 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEPOCKET_HPP
# define EULEEPOCKET_HPP

# include "EuleeWallet.hpp"

# define RESET		"\033[0m"
# define RED		"\033[1;31m"
# define GREEN		"\033[1;32m"
# define BLUE		"\033[1;34m"
# define MAGENTA	"\033[1;35m"
# define CYAN		"\033[1;36m"
# define BLUENORM	"\033[34m"
# define CYANNORM	"\033[36m"

enum	Mode
{
	READ,
	WRITE
};

class EuleePocket
{
	public:
		typedef Key								key_type;
		typedef std::vector<std::string>		mapped_type;
		typedef std::map<key_type, mapped_type>	value_type;
		typedef	value_type::iterator			iterator;

		EuleePocket();
		EuleePocket(EuleeWallet server, std::vector<EuleeWallet> vectorLocation, std::map<int, std::string> errorPage);
		~EuleePocket();

		mapped_type	&operator[](key_type key);

		iterator	begin();
		iterator	end();

		std::vector<EuleeWallet>			vectorLocation;
		std::map<std::string, EuleeWallet>	location;
		std::map<int, std::string>			errorPage;
		int									portIndex;

	private:
		EuleeWallet							_server;

};

#endif