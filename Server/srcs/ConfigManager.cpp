/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   ConfigManager.cpp                                                        */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 01:59:12 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 15:38:11 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ConfigManager.hpp"

ConfigManager::ConfigManager(void): _configFilePath() {}

ConfigManager::ConfigManager(std::string configFilePath): _configFilePath(configFilePath) {}

ConfigManager::~ConfigManager() {}

ConfigManager	&ConfigManager::operator=(const ConfigManager &ref)
{
	this->_configFilePath = ref._configFilePath;
	return (*this);
}

void	ConfigManager::_createToken(std::string *token, Type *currentType, Type type, std::string c, int lineNum)
{
	if (token->empty() == false)
	{
		this->_tokens.push_back(Token(*token, *currentType, lineNum));
		token->clear();
	}
	this->_tokens.push_back(Token(c, type, lineNum));
	*currentType = CONTEXT;
}

void	ConfigManager::_lexLine(std::string line, int lineNum)
{
	std::string	token;
	Type		type = CONTEXT;
	int			count = 0;

	for (int i = 0; line[i] != '\0'; i++)
	{
		if (line[i] == '#')
			break;
		else if (line[i] == '{')
			this->_createToken(&token, &type, OPEN_BRACE, "{", lineNum);
		else if (line[i] == '}')
			this->_createToken(&token, &type, CLOSE_BRACE, "}", lineNum);
		else if (line[i] == ';')
		{
			this->_createToken(&token, &type, SEMICOLON, ";", lineNum);
			count = 0;
		}
		else if (std::string(" \f\n\r\t\v").find(line[i]) != std::string::npos)
		{
			if (token.empty() == false)
			{
				type = (count == 0) ? KEY : VALUE;
				count++;
				this->_tokens.push_back(Token(token, type, lineNum));
				token.clear();
			}
			type = VALUE;
		}
		else
			token.push_back(line[i]);
	}
	if (token.empty() == false)
		this->_tokens.push_back(Token(token, type, lineNum));
}

void	ConfigManager::parseConfigFile()
{
	if (this->_configFilePath.empty())
		throw std::runtime_error("Failed to open config file");
	std::ifstream	file(this->_configFilePath.c_str(), std::ios::binary);
	if (file.is_open() == false)
		throw std::runtime_error("Failed to open config file");

	std::string	line;
	int			lineNum = 0;
	while (std::getline(file, line))
		this->_lexLine(line, ++lineNum);
	file.close();
}

void	ConfigManager::printTokens(void)
{
	for (size_t i = 0; i < this->_tokens.size(); i++)
		std::cout << "Token: |" << this->_tokens[i].token << "| Type: |" << this->_tokens[i].type << "| Line: |" << this->_tokens[i].lineNum << "|" << std::endl;
}

void	ConfigManager::printError(std::string str, int i)
{

	std::cout << str << "Line: " << this->_tokens[i].lineNum << std::endl;
	std::cout << this->_tokens[i].token << std::endl;
	exit(1);
}

void	ConfigManager::configLibrary(void)
{
	const char	*serverlib[11] = {"server", "location", "listen", "root", "index", "server_name", "error_page", "client_max_body_size", "auto_index", "return", "cgi"};
	const char	*locationlib[12] = {"root", "index", "include", "cgi", "upload", "error_page", "client_max_body_size", "auto_index", "return", "limit_except", "allow", "deny"};

	this->_serverVar = std::vector<std::string>(serverlib, serverlib + 11);
	this->_locationVar = std::vector<std::string>(locationlib, locationlib + 12);
}

int		ConfigManager::checkValue(int i, int previous)
{
	if (this->_tokens[i].type == VALUE) // 2
	{
		if (previous == 1 || previous == 2)
			previous = VALUE;
		else
			printError("Not a key/value before another value. ", i);
	}
	return (previous);
}

int	ConfigManager::checkSemicolon(int i, int previous)
{
	if (this->_tokens[i].type == SEMICOLON) // 3
	{
		if (previous == 2)
			previous = SEMICOLON;
		else
			printError("Semicolon not after value. ", i);
	}
	return (previous);
}

int		ConfigManager::checkOpenBrace(int i, int previous, int *braces, int main_block)
{
	if (this->_tokens[i].type == OPEN_BRACE) // 4
	{
		if (main_block > 0)
			*braces += 1;
		if (previous == 1 || previous  == 2)
			previous = OPEN_BRACE;
		else
			printError("Open braces not after key or value. ", i);
	}
	return (previous);
}

int		ConfigManager::checkCloseBrace(int i, int previous, int *braces, int *main_block)
{
	if (this->_tokens[i].type == CLOSE_BRACE) // 5
	{
		*braces -= 1;
		if (*braces == 0)
			*main_block -= 1;
		if (previous == 3 || previous == 5)
			previous = CLOSE_BRACE;
		else
			printError("Close braces not after close braces or semicolon. ", i);

	}
	return (previous);
}

void	ConfigManager::checkImportantCheck(int i)
{
	if (this->_tokens[i].token == "root" || this->_tokens[i].token == "auto_index" || this->_tokens[i].token == "client_max_body_size")
	{
		if (this->_tokens[i + 2].type == VALUE)
			printError("I hardcoded these 3 to obtain this error, problem?. ", i + 2);
	}
}

int		ConfigManager::checkLocationKey(size_t i, int previous, int *braces, int *main_block)
{
	if (this->_tokens[i].type == KEY) // 1
	{
		if (*main_block == 1)
		{
			if (!(std::find(this->_locationVar.begin(), this->_locationVar.end(),
				this->_tokens[i].token) != this->_locationVar.end())) // server
				printError("Not a valid string for Location block. ", i);
		}

		if ((previous == 3 || previous == 4 || previous == 5) || *main_block == 0)
			previous = KEY;
		else
			printError("Key is not after braces or semicolon. ", i);

		this->checkImportantCheck(i);

		if (*main_block == 0)
		{
			if (*braces > 0)
				printError("Invalid number of braces.", i);
			*main_block = 1;
		}
		// if (!(std::find(this->_locationVar.begin(), this->_locationVar.end(),
		// 	this->_tokens[i].token) != this->_locationVar.end())) // server
		// 	printError("Not a valid string for Location block. ", i);
	}
	return (previous);
}



int		ConfigManager::checkServerKey(size_t i, int previous, int *braces, int *main_block)
{

	if (this->_tokens[i].type == KEY) // 1
	{
		if ((previous == 3 || previous == 4 || previous == 5) || *main_block == 0)
			previous = KEY;
		else
			printError("Key is not after braces or semicolon. ", i);

		this->checkImportantCheck(i);
		if (*main_block == 1 && this->_tokens[i].token == this->_serverVar[0])
				printError("Server cannot be non-main directive. ", i);

		if (!(std::find(this->_serverVar.begin(), this->_serverVar.end(),
			this->_tokens[i].token) != this->_serverVar.end())) // server
			printError("Not a valid string for Server block. ", i);

		if (*main_block == 0)
		{
			if (this->_tokens[i].token != this->_serverVar[0])
				printError("Invalid main directive (should be server). ", i);
			if (*braces > 0)
				printError("Invalid number of braces.", i);
			*main_block = 1;
		}
		
		if (this->_tokens[i].token == this->_serverVar[1]) // location
		{
			if (this->_tokens[i + 2].type != OPEN_BRACE)
				printError("Location should only have one value. ", i + 2);
			*main_block = 2;
		}
	}
	return (previous);
}

int	ConfigManager::locationBlock(size_t i)
{
	int		loc_block = 0;
	int		braces = 0;
	int		previous = 0;

	while (!(previous == 5 && braces == 0))
	{
		previous = this->checkLocationKey(i, previous, &braces, &loc_block);
		previous = this->checkValue(i, previous);
		previous = this->checkSemicolon(i, previous);
		previous = this->checkOpenBrace(i, previous, &braces, loc_block);
		previous = this->checkCloseBrace(i , previous, &braces, &loc_block);
		i += 1;
	}
	return (i);
}
void	ConfigManager::errorHandleShit(void)
{
	size_t	i = 0;
	int		main_block = 0;
	int		braces = 0;
	int		previous = 0;

	while (i < this->_tokens.size())
	{
		if (this->_tokens[i].type == CONTEXT) // 0
			printError("Random word out of blocks. ", i);
		previous = this->checkServerKey(i, previous, &braces, &main_block);
		if (main_block == 2) // location block
		{
			i = this->locationBlock(i);
			main_block = 1;
			previous = 5;
			continue ;
		}
		previous = this->checkValue(i, previous);
		previous = this->checkSemicolon(i, previous);
		previous = this->checkOpenBrace(i, previous, &braces, main_block);
		previous = this->checkCloseBrace(i , previous, &braces, &main_block);
		i++;
	}
	if (braces != 0)
		printError("Invalid number of brace. ", this->_tokens.size() - 1);
	return ;
}

std::vector<Token>	&ConfigManager::getToken(void)
{
	return (this->_tokens);
}