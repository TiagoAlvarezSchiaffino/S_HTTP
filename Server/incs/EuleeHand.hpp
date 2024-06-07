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
/*   Updated: 2024/06/07 05:58:40 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEHAND_HPP
# define EULEEHAND_HPP

# include "EuleePocket.hpp"
# include "ConfigManager.hpp"
# include "CookieJar.hpp"

class EuleeHand
{
	public:
		EuleeHand();
		EuleeHand(std::string configFilePath, const ConfigManager &configManager);
		~EuleeHand();

		int			checkPath(std::string path, int	isFile, int isDirectory);
		int			sendHttp(int statusCode, std::string responseBody = "");
		int			isCGI();
		int			checkExcept();
		int			checkClientBodySize();
		int			parseHeader();
		int			unchunkResponse();
		void		printTokens();
		void		parseConfigFile();
		void		configLibrary();
		void		errorHandleShit();
		void		printServers();
		void		parseConfigServer();
		void		perrorExit(std::string msg, int exitTrue = 1);
		void		convertLocation();
		size_t		addEnv(std::string input);
		size_t		clearEnv();
		std::string	cgiPath();
		std::string	extractHTML(std::string path);
		std::string directoryListing(std::string path);

		char								**envp;
		std::map<std::string, std::string>	cgi;
		std::map<int, std::string>			statusList, buffer, response, method, methodPath, locationPath;
		std::map<int, long>					bytes_sent, serverIndex, useDefaultIndex, useDirectoryListing;
		std::map<int, bool>					parsed, cookieExist;
		std::vector<EuleePocket>			server;
		std::vector<int>					serverFd;
		std::vector<sockaddr_in>			serverAddr;
		int									socket, connectionCount;
		fd_set								myReadFds, myWriteFds;
		CookieJar							cookieJar;

	private:
		size_t			_envpSize;
		std::string		_configFilePath;
		ConfigManager	_configManager;

		int				_unchunkIntofile(int fd, std::string buffer, int isHeader);
		size_t			_readFile(std::string *buffer1, std::string *buffer2, int infile, char *temp, long bytes_read, int type, int *count);
		size_t			_parseServer(std::vector<Token> &tokens, size_t i);
		size_t			_parseErrorPage(std::vector<Token> &tokens, size_t i, std::map<int, std::string> &err);
		size_t			_parseCgi(std::vector<Token> &tokens, size_t i, EuleeWallet &location, int blockType);
		size_t			_parseLocation(std::vector<Token> &tokens, std::vector<EuleeWallet> &location, size_t i);
		size_t			_parsingHelper(std::vector<Token> &tokens, size_t i, EuleeWallet &location, std::string needle, Key key);
		std::string 	_getFileCreationTime(const std::string &path, const std::string &file_name);
		std::string 	_getFileSize(const std::string &path, const std::string &file_name);
		
};

#endif