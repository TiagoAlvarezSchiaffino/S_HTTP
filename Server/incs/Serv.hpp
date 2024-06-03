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
/*   Updated: 2024/06/03 14:39:02 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Serv_HPP
# define Serv_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <unistd.h>
# include <netdb.h>
# include <poll.h>

# include "../incs/ConfigManager.hpp"
# include "../incs/HttpPostResponse.hpp"
# include "../incs/HttpDefaultResponse.hpp"
# include "server.hpp"

# define WS_DOMAIN		AF_INET
# define WS_TYPE		SOCK_STREAM
# define WS_PROTOCOL	0
# define WS_FLAGS		AI_PASSIVE
# define WS_SERVER_NAME	"localhost"
# define WS_BACKLOG		10
# define WS_PORT		8080
# define WS_BUFFER_SIZE	30000
# define WS_TIMEOUT		3000

class Serv
{
	public:
		Serv(std::string configFilePath);
		~Serv();
		void	runServer();

	private:
		void				_perrorExit(std::string msg);
		void				_setupServer();
		void				_handleCgi(std::string method, int contentLength);
		int					_handleGet();
		void				_serverLoop();

		std::string			_configFilePath, _path;
		int					_serverFd, _newSocket, _ret;
		struct sockaddr_in	_serverAddr;
		struct pollfd		_fds[1];
		ConfigManager		_configManager;
};

#endif