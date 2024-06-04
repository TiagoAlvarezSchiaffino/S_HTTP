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
/*   Updated: 2024/06/04 10:53:23 by Tiago                  /_____/ U         */
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
		EuleeHand(const EuleeHand &ref);
		~EuleeHand(void);

		EuleeHand	&operator=(const EuleeHand &ref);

		void	printTokens(void);
		void	parseConfigFile(void);
		void	configLibrary(void);
		void	errorHandleShit(void);
		void	printServers(void);
		void	parseConfigServer(void);

		std::vector<EuleePocket>	server;

	private:
		std::string		_configFilePath;
		ConfigManager	_configManager;
		size_t			_parseServer(std::vector<Token> &tokens, size_t i);
		size_t			_parseLocation(std::vector<Token> &tokens, std::vector<EuleeWallet> &location, size_t i);
		size_t			_parsingHelper(std::vector<Token> &tokens, size_t i, EuleeWallet &location, std::string needle, Key key);
};

#endif