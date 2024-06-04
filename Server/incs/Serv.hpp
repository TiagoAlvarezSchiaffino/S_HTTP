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
/*   Updated: 2024/06/04 05:55:12 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Serv_HPP
# define Serv_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <vector>
# include <unistd.h>
# include <netdb.h>
# include <fcntl.h>

# include "../incs/ConfigManager.hpp"
# include "../incs/HttpPostResponse.hpp"
# include "../incs/HttpDefaultResponse.hpp"
# include "../incs/HttpCgiResponse.hpp"
# include "../incs/HttpGetResponse.hpp"
# include "server.hpp"

# define WS_DOMAIN		AF_INET
# define WS_TYPE		SOCK_STREAM
# define WS_PROTOCOL	0
# define WS_FLAGS		AI_PASSIVE
# define WS_SERVER_NAME	"localhost"
# define WS_BACKLOG		10
# define WS_PORT		8081
# define WS_BUFFER_SIZE	30000
# define WS_TIMEOUT		3

/* TO BE REMOVED */
enum	Mode
{
	READ,
	WRITE
};
class Serv
{
	public:
		Serv(std::string configFilePath);
		~Serv();
		void	runServer();

	private:
		void				_perrorExit(std::string msg, int exitTrue = 1);
		void				_setupServer();
		void				_serverLoop();
		long				ft_select2(int fd, void *buffer, size_t size, Mode mode);

		std::string					_configFilePath, _path;
		std::vector<int>			_serverFd;
		int							_socket;
		std::vector<sockaddr_in>	_serverAddr;
		ConfigManager				_configManager;
};

#endif