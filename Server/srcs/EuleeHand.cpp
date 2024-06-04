/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeHand.cpp                                                            */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:20:49 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 10:29:40 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand(void) : _configFilePath(), _configManager() {}

EuleeHand::EuleeHand(std::string configFilePath, ConfigManager const &configManager) : _configFilePath(configFilePath), _configManager(configManager) {}

EuleeHand::EuleeHand(const EuleeHand &ref) : _configFilePath(ref._configFilePath), _configManager(ref._configManager) {}

EuleeHand::~EuleeHand(void) {}

EuleeHand	&EuleeHand::operator=(const EuleeHand &ref)
{
	this->_configFilePath = ref._configFilePath;
	this->_configManager = ref._configManager;
	return (*this);
}

void	EuleeHand::printTokens(void)
{
	this->_configManager.printTokens();
}

void	EuleeHand::parseConfigFile(void)
{
	this->_configManager.parseConfigFile();
}

void	EuleeHand::configLibrary(void)
{
	this->_configManager.configLibrary();
}

void	EuleeHand::errorHandleShit(void)
{
	this->_configManager.errorHandleShit();
}

void	EuleeHand::printServers(void)
{
	for (size_t i = 0; i < this->server.size(); ++i)
	{
		std::cout << "=======================" << std::endl;
		std::cout << CYAN "server " << i + 1 << RESET << std::endl;
		for (EuleePocket::iterator it = this->server[i].begin(); it != this->server[i].end(); ++it)
		{
			std::cout << CYANNORM << it->first << " : ";
			for (size_t k = 0; k < it->second.size(); ++k)
			{
				std::cout << it->second[k];
				if (k + 1 < it->second.size())
					std::cout << " ";
			}
			std::cout << std::endl;
		}
		std::cout << RESET << std::endl;
		for (size_t j = 0; j < this->server[i].location.size(); ++j)
		{
			std::cout << BLUE "location " << j + 1 << RESET << std::endl;
			for (EuleePocket::iterator it2 = this->server[i].location[j].begin(); it2 != this->server[i].location[j].end(); ++it2)
			{
				std::cout << BLUENORM << it2->first << " : ";
				for (size_t d = 0; d < it2->second.size(); ++d)
				{
					std::cout << it2->second[d];
					if (d + 1 < it2->second.size())
						std::cout << " ";
				}
				std::cout << std::endl;
			}
			if (j + 1 < this->server[i].location.size())
				std::cout << std::endl;
		}
		std::cout << RESET;
		std::cout << "=======================" << std::endl;
	}
}

size_t	EuleeHand::_parsingHelper(std::vector<Token> &tokens, size_t i, EuleeWallet &location, std::string needle, Key key)
{
	if (tokens[i].token == needle)
	{
		while (tokens[++i].token != ";")
			location[key].push_back(tokens[i].token);
	}
	return (i);
}

size_t	EuleeHand::_parseLocation(std::vector<Token> &tokens, std::vector<EuleeWallet> &location, size_t i)
{
	EuleeWallet	loc;

	if (tokens[i].token != "{")
		loc[LOCATION_READ_PATH].push_back(tokens[i++].token);
	while (tokens[i].token != "}")
	{
		i = this->_parsingHelper(tokens, i, loc, "cgi", CGI);
		i = this->_parsingHelper(tokens, i, loc, "root", ROOT);
		i = this->_parsingHelper(tokens, i, loc, "index", INDEX);
		i = this->_parsingHelper(tokens, i, loc, "return", RETURN);
		i = this->_parsingHelper(tokens, i, loc, "upload", UPLOAD);
		i = this->_parsingHelper(tokens, i, loc, "include", INCLUDE);
		i = this->_parsingHelper(tokens, i, loc, "error_page", ERROR_PAGE);
		i = this->_parsingHelper(tokens, i, loc, "auto_index", AUTO_INDEX);
		i = this->_parsingHelper(tokens, i, loc, "limit_except", LIMIT_EXCEPT);
		i = this->_parsingHelper(tokens, i, loc, "client_max_body_size", CLIENT_MAX_BODY_SIZE);
		++i;
	}
	location.push_back(EuleeWallet(loc));
	return (i);
}

size_t	EuleeHand::_parseServer(std::vector<Token> &tokens, size_t i)
{
	EuleeWallet					serv;
	std::vector<EuleeWallet>	location;

	while (i < tokens.size() && tokens[i].token != "server")
	{
		i = this->_parsingHelper(tokens, i, serv, "cgi", CGI);
		i = this->_parsingHelper(tokens, i, serv, "root", ROOT);
		i = this->_parsingHelper(tokens, i, serv, "index", INDEX);
		i = this->_parsingHelper(tokens, i, serv, "listen", LISTEN);
		i = this->_parsingHelper(tokens, i, serv, "return", RETURN);
		i = this->_parsingHelper(tokens, i, serv, "upload", UPLOAD);
		i = this->_parsingHelper(tokens, i, serv, "include", INCLUDE);
		i = this->_parsingHelper(tokens, i, serv, "auto_index", AUTO_INDEX);
		i = this->_parsingHelper(tokens, i, serv, "error_page", ERROR_PAGE);
		i = this->_parsingHelper(tokens, i, serv, "server_name", SERVER_NAME);
		i = this->_parsingHelper(tokens, i, serv, "limit_except", LIMIT_EXCEPT);
		i = this->_parsingHelper(tokens, i, serv, "client_max_body_size", CLIENT_MAX_BODY_SIZE);
		if (tokens[i].token == "location")
			i = this->_parseLocation(tokens, location, ++i);
		++i;
	}
	this->server.push_back(EuleePocket(serv, location));
	return (++i);
}

void	EuleeHand::parseConfigServer(void)
{
	std::vector<Token>	tokens = this->_configManager.getToken();

	// i = 1; because tokens[0].token is always equals to "server"
	size_t	i = 1;
	while (i < tokens.size())
		i = this->_parseServer(tokens, i);
}