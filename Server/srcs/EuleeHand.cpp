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
/*   Updated: 2024/06/04 08:09:30 by Tiago                  /_____/ U         */
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
		std::cout << CYAN;
		std::cout << "server " << i + 1 << std::endl;
		std::cout << RESET << CYANNORM;
		std::cout << "port       : " << this->server[i].port << std::endl;
		std::cout << "root       : " << this->server[i].root << std::endl;
		std::cout << "index      : " << this->server[i].index << std::endl;
		std::cout << "domain     : " << this->server[i].domain << std::endl;
		std::cout << "autoIndex  : " << this->server[i].autoIndex << std::endl;
		std::cout << "uploadSize : " << this->server[i].uploadSize << std::endl;
		std::cout << "errorPath  : " << this->server[i].errorPath << std::endl;
		std::cout << std::endl;
		for (size_t j = 0; j < this->server[i].location.size(); ++j)
		{
			std::cout << RESET << BLUE;
			std::cout << "location " << j + 1 << std::endl;
			std::cout << RESET << BLUENORM;
			std::cout << "readType   : " << this->server[i].location[j].priority << std::endl;
			std::cout << "readPath   : " << this->server[i].location[j].readPath << std::endl;
			std::cout << "root       : " << this->server[i].location[j].root << std::endl;
			std::cout << "index      : " << this->server[i].location[j].index << std::endl;
			std::cout << "cgi_pass   : " << this->server[i].location[j].cgiPass << std::endl;
			std::cout << "cgi_index  : " << this->server[i].location[j].cgiIndex << std::endl;
			std::cout << "cgi_param  : " << this->server[i].location[j].cgiParam << std::endl;
			if (j + 1 < this->server[i].location.size())
				std::cout << std::endl;
		}
		std::cout << RESET;
		std::cout << "=======================" << std::endl;
	}
}

size_t	EuleeHand::_parsingHelper(std::vector<Token> &tokens, size_t i, std::string &val, std::string needle)
{
	if (tokens[i].token == needle)
		val = tokens[++i].token;
	return (i);
}

size_t	EuleeHand::_LocationPriorityCheck(std::vector<Token> &tokens, size_t i, RegularExpression &priority, std::string &readPath)
{
	if (tokens[i].token == "~")
		priority = CASE_SENSITIVE;
	else if (tokens[i].token == "~*")
		priority = CASE_INSENSITIVE;
	else if (tokens[i].token == "^~")
		priority = IGNORE_OTHERS;
	else if (tokens[i].token == "=")
		priority = EXACT_MATCH;
	else
		priority = NOT_SPECIFIED;
	if (tokens[++i].token != "{")
		readPath = tokens[i].token;
	return (i);
}

size_t	EuleeHand::_parseLocation(std::vector<Token> &tokens, std::vector<EuleeWallet> &location, size_t i)
{
	std::string			readPath, root, index, include, cgiPass, cgiIndex, cgiParam;
	RegularExpression	priority;

	i = this->_LocationPriorityCheck(tokens, i, priority, readPath);
	while (tokens[i].token != "}")
	{
		i = this->_parsingHelper(tokens, i, root, "root");
		i = this->_parsingHelper(tokens, i, index, "index");
		i = this->_parsingHelper(tokens, i, include, "include");
		i = this->_parsingHelper(tokens, i, cgiPass, "cgi_pass");
		i = this->_parsingHelper(tokens, i, cgiIndex, "cgi_index");
		i = this->_parsingHelper(tokens, i, cgiParam, "cgi_param");
		++i;
	}
	location.push_back(EuleeWallet(priority, readPath, root, index, include, cgiPass, cgiIndex, cgiParam));
	return (i);
}

size_t	EuleeHand::_parseServer(std::vector<Token> &tokens, size_t i)
{
	std::string	port, root, index, domain, autoIndex, uploadSize, errorPath;
	std::vector<EuleeWallet>	location;

	while (i < tokens.size() && tokens[i].token != "server")
	{
		i = this->_parsingHelper(tokens, i, port, "listen");
		i = this->_parsingHelper(tokens, i, root, "root");
		i = this->_parsingHelper(tokens, i, index, "index");
		i = this->_parsingHelper(tokens, i, domain, "server_name");
		i = this->_parsingHelper(tokens, i, errorPath, "error_page");
		i = this->_parsingHelper(tokens, i, uploadSize, "client_max_body_size");
		i = this->_parsingHelper(tokens, i, autoIndex, "auto_index");
		if (tokens[i].token == "location")
			i = this->_parseLocation(tokens, location, ++i);
		++i;
	}
	this->server.push_back(EuleePocket(port, root, index, domain, autoIndex, uploadSize, errorPath, location));
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