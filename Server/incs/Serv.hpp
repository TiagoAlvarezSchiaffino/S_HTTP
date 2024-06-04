/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Serv.hpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:48:14 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 12:48:00 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Serv_HPP
# define Serv_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <fcntl.h>

# include "ConfigManager.hpp"
# include "EuleeHand.hpp"
# include "EuleeWallet.hpp"
# include "HttpPostResponse.hpp"
# include "HttpDefaultResponse.hpp"
# include "HttpCgiResponse.hpp"
# include "HttpGetResponse.hpp"
# include "HttpDeleteResponse.hpp"
# include "HttpHeadResponse.hpp"
# include "server.hpp"
# include "HttpPutResponse.hpp"

# define WS_BACKLOG				10
# define WS_PORT				8080
# define WS_BUFFER_SIZE			30000
# define WS_TIMEOUT				3
# define DEFAULT_CONFIG_PATH	"conf/default.conf"

/* TO BE REMOVED */
enum	Mode
{
	READ,
	WRITE
};
#include <string>
void	perrorExit(std::string msg, int exitTrue = 1);
long	ft_select(int fd, void *buffer, size_t size, Mode mode);

class Serv
{
	public:
		Serv(std::string configFilePath);
		~Serv();
		void						runServer();

	private:
		void						_setupServer();
		int							_unchunkResponse();
		void						_serverLoop();

		std::string					_configFilePath, _path, _buffer;
		std::vector<int>			_serverFd;
		int							_socket;
		std::vector<sockaddr_in>	_serverAddr;
		ConfigManager				_configManager;
};

#endif