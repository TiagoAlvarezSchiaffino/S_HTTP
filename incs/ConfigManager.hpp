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
/*   Updated: 2024/05/16 03:56:22 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGMANAGER_HPP
# define CONFIGMANAGER_HPP

# include <string>
# include <fstream>
# include <vector>
# include <iostream>

# include "Token.hpp"

class ConfigManager
{
	public:
		ConfigManager();
		ConfigManager(std::string configFilePath);
		ConfigManager	&operator=(const ConfigManager &srcs);
		~ConfigManager();
		void			parseConfigFile();

	private:
		void						_lexLine(std::string line, int lineNum);
		void						_createToken(std::string *token, Type *currentType, Type type, std::string c, int lineNum);
		std::ifstream				_file;
		std::string					_configFilePath;
		std::vector<Token>			_tokens;
};

#endif