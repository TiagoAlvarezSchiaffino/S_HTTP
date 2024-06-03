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
/*   Updated: 2024/06/03 15:58:39 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEHAND_HPP
# define EULEEHAND_HPP

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
		void	errorHandleShit(void);
		void	configLibrary(void);

	private:
		std::string		_configFilePath;
		ConfigManager	_configManager;
};

#endif