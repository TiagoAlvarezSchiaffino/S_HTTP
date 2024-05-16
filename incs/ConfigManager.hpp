/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   ConfigManager.hpp                                                        */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 01:53:07 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 01:54:46 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGMANAGER_HPP
# define CONFIGMANAGER_HPP

# include <string>
# include <fstream>

# define ValidTokens "{};"
# define ValidSpaces " \f\n\r\t\v"

class ConfigManager
{
	public:
		ConfigManager();
		ConfigManager(std::string configFilePath);
		ConfigManager	&operator=(const ConfigManager &srcs);
		~ConfigManager();
		int	parseConfigFile();

	private:
		std::ifstream	_file;
		std::string		_configFilePath;
		std::string		_fileBuffer;
};

#endif