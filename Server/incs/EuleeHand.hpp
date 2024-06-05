/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeHand.hpp                                                            */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:12:03 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 13:35:20 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEHAND_HPP
# define EULEEHAND_HPP

# include "EuleePocket.hpp"
# include "ConfigManager.hpp"

# include <iostream>
# include <sstream>
# include <dirent.h>
# include <sys/stat.h>

# define WS_BUFFER_SIZE			10000
# define WS_TIMEOUT				0
# define WS_ERROR_PAGE_PATH 	"./html/server_html/error.html"
# define WS_DEFAULT_PAGE_PATH	"./html/server_html/default.html"

class EuleeHand
{
	public:
		EuleeHand();
		EuleeHand(std::string configFilePath, const ConfigManager &configManager, char **envp);
		~EuleeHand();

		int			checkPath(std::string path, int	isFile, int isDirectory);
		int			sendHttp(int statusCode, int closeSocket = 0, std::string htmlPath = "");
		void		printTokens();
		void		parseConfigFile();
		void		configLibrary();
		void		errorHandleShit();
		void		printServers();
		void		parseConfigServer();
		void		perrorExit(std::string msg, int exitTrue = 1);
		long		ft_select(int fd, void *buff, size_t size, Mode mode);
		std::string	extractHTML(std::string path);

		int			isCGI();
		int			checkExcept();
		int			unchunkResponse();
		int			checkClientBodySize();
		void		convertLocation();
		std::string	cgiPath();
		std::string directoryListing(std::string path);
		

		char								**envp;
		std::map<std::string, std::string>	cgi;
		std::map<int, std::string>			statusList;
		std::vector<EuleePocket>			server;
		std::vector<int>					serverFd;
		std::vector<sockaddr_in>			serverAddr;
		int									socket, serverIndex, useDefaultIndex, useDirectoryListing;
		std::string							method, methodPath, buffer, locationPath;

	private:
		size_t			_parseServer(std::vector<Token> &tokens, size_t i);
		size_t			_parseCgi(std::vector<Token> &tokens, size_t i, EuleeWallet &location, int blockType);
		size_t			_parseLocation(std::vector<Token> &tokens, std::vector<EuleeWallet> &location, size_t i);
		size_t			_parsingHelper(std::vector<Token> &tokens, size_t i, EuleeWallet &location, std::string needle, Key key);
		std::string		_configFilePath;
		std::string 	_getFileSize(const std::string &path, const std::string &file_name);
		std::string 	_getFileCreationTime(const std::string &path, const std::string &file_name);
		ConfigManager	_configManager;
		
};

#endif