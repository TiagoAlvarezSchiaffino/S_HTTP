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
/*   Updated: 2024/06/04 13:34:08 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEHAND_HPP
# define EULEEHAND_HPP

# include "EuleePocket.hpp"
# include "ConfigManager.hpp"

class EuleeHand
{
	public:
		EuleeHand(void);
		EuleeHand(std::string configFilePath, const ConfigManager &configManager);
		~EuleeHand(void);

		void	printTokens(void);
		void	parseConfigFile(void);
		void	configLibrary(void);
		void	errorHandleShit(void);
		void	printServers(void);
		void	parseConfigServer(void);
		void	perrorExit(std::string msg, int exitTrue = 1);
		long	ft_select(int fd, void *buff, size_t size, Mode mode);
		int		sendHttp(std::string statusCode, std::string path);

		int							socket;
		std::map<std::string, std::string>	envp, cgi , statusCode;
		std::vector<EuleePocket>	server;
		std::vector<int>			serverFd;
		std::vector<sockaddr_in>	serverAddr;
		std::string					methodPath, buffer;

	private:
		std::string		_configFilePath;
		ConfigManager	_configManager;
		size_t			_parseServer(std::vector<Token> &tokens, size_t i);
		size_t			_parseLocation(std::vector<Token> &tokens, std::vector<EuleeWallet> &location, size_t i);
		size_t			_parsingHelper(std::vector<Token> &tokens, size_t i, EuleeWallet &location, std::string needle, Key key);
};

#endif