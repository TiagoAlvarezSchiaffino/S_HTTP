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
/*   Updated: 2024/06/04 15:15:39 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEPOCKET_HPP
# define EULEEPOCKET_HPP

# include <unistd.h>
# include <sys/select.h>
# include <netinet/in.h>
# include "EuleeWallet.hpp"

# define RESET		"\033[0m"
# define RED		"\033[1;31m"
# define GREEN		"\033[1;32m"
# define BLUE		"\033[1;34m"
# define MAGENTA	"\033[1;35m"
# define CYAN		"\033[1;36m"
# define BLUENORM	"\033[34m"
# define CYANNORM	"\033[36m"
# define WS_TIMEOUT	3

enum	Mode
{
	READ,
	WRITE
};

// server block
class EuleePocket
{
	public:
		typedef Key								key_type;
		typedef std::vector<std::string>		mapped_type;
		typedef std::map<key_type, mapped_type>	value_type;
		typedef	value_type::iterator			iterator;

		EuleePocket(void);
		EuleePocket(EuleeWallet server, std::vector<EuleeWallet> vectorLocation);
		~EuleePocket(void);

		mapped_type	&operator[](key_type key);

		// member function
		iterator	begin(void);
		iterator	end(void);

		// member variable
        std::map<std::string, std::string>	envp;
		std::vector<EuleeWallet>			vectorLocation;
		std::map<std::string, EuleeWallet>	location;
		int									portIndex;

	private:
		std::map<std::string, std::string>	_envp;
		EuleeWallet							_server;

};

#endif