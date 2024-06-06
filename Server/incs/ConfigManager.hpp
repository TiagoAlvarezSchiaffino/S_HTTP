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
/*   Updated: 2024/06/06 05:40:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGMANAGER_HPP
# define CONFIGMANAGER_HPP

# include "Token.hpp"

class ConfigManager
{
	public:
		ConfigManager();
		ConfigManager(std::string configFilePath);
		~ConfigManager();

		ConfigManager		&operator=(const ConfigManager &ref);

		void			parseConfigFile();
		void			printTokens();
		void			configLibrary();

		void			checkImportantCheck(int i);
		bool			checkCompulsories(int i);

		int				checkServerKey(size_t i, int previous, int *braces, int *main_block);
		int				checkLocationKey(size_t i, int previous, int *braces, int *main_block);
		int				checkValue(int i, int previous);
		int				checkSemicolon(int i, int previous);
		int				checkOpenBrace(int i, int previous, int *braces, int main_block);
		int				checkCloseBrace(int i, int previous, int *braces, int *main_block);

		int				locationBlock(size_t i);
		void			errorHandleShit();

		// Utils
		void				printError(std::string str, int i);
		std::vector<Token>	&getToken();

	private:
		void						_lexLine(std::string line, int lineNum);
		void						_createToken(std::string *token, Type *currentType, Type type, std::string c, int lineNum);

		std::string					_configFilePath;
		std::vector<Token>			_tokens;
		std::vector<std::string>	_validStr;
		std::vector<std::string>	_serverVar;
		std::vector<std::string>	_locationVar;

};

#endif