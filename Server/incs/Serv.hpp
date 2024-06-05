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
/*   Updated: 2024/06/04 18:52:46 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
# define SERV_HPP

# include "server.hpp"
# include "EuleeHand.hpp"
# include "EuleeWallet.hpp"
# include "ConfigManager.hpp"
# include "HttpPostResponse.hpp"
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

class Server
{
	public:
		Server(std::string configFilePath, char **envp);
		~Server();
		void			runServer();

	private:
		void			_setupServer();
		void			_acceptConnection();
		void			_receiveRequest();
		void			_writeResponse();
		void			_serverLoop();
		void			_doRequest();
		int				_parseRequest();
		int				_handleFavicon();
		int				_handleRedirection();

		ConfigManager	_configManager;
		EuleeHand		_database;
};

#endif