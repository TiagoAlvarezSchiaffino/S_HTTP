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
/*   Updated: 2024/06/05 13:12:09 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
# define SERV_HPP

# include <filesystem>
# include <iostream>
# include <sstream>
# include <fstream>
# include <cstring>
# include <string>
# include <limits>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <fcntl.h>

# include "server.hpp"
# include "ConfigManager.hpp"
# include "EuleeHand.hpp"
# include "EuleeWallet.hpp"
# include "HttpPostResponse.hpp"
# include "HttpDefaultResponse.hpp"
# include "HttpCgiResponse.hpp"
# include "HttpGetResponse.hpp"
# include "HttpDeleteResponse.hpp"
# include "HttpHeadResponse.hpp"
# include "HttpPutResponse.hpp"

# define WS_BACKLOG				10
# define WS_BUFFER_SIZE			100000
# define WS_TESTER_SIZE			100000
# define WS_TEMP_FILE_IN		".tempIn"
# define WS_TEMP_FILE_OUT		".tempOut"
# define DEFAULT_CONFIG_PATH	"conf/default.conf"

class Serv
{
	public:
		Serv(std::string configFilePath, char **envp);
		~Serv();
		void			runServer();

	private:
		void			_setupServer();
		void			_acceptConnection();
		int				_receiveRequest();
		int				_handleFavicon();
		int				_handleRedirection();
		void			_serverLoop();

		ConfigManager	_configManager;
		EuleeHand		_database;
};

#endif