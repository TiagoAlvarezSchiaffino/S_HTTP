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
/*   Updated: 2024/06/04 15:52:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand(void) : socket(), serverIndex(), useDefaultIndex(), server(), serverFd(), serverAddr(), methodPath(), buffer(), _configFilePath(), _configManager() {}

EuleeHand::EuleeHand(std::string configFilePath, ConfigManager const &configManager) :  socket(), serverIndex(), useDefaultIndex(), server(), serverFd(), serverAddr(), methodPath(), buffer(), _configFilePath(configFilePath), _configManager(configManager) {}

EuleeHand::~EuleeHand(void) {}

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
		for (size_t j = 0; j < this->server[i].vectorLocation.size(); ++j)
		{
			std::cout << BLUE "location " << j + 1 << RESET << std::endl;
			for (EuleePocket::iterator it2 = this->server[i].vectorLocation[j].begin(); it2 != this->server[i].vectorLocation[j].end(); ++it2)
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
			if (j + 1 < this->server[i].vectorLocation.size())
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

	size_t	i = 1;
	while (i < tokens.size())
		i = this->_parseServer(tokens, i);
	std::map<std::string, std::string>	unique;
	for (size_t n = 0; n < this->server.size(); ++n)
	{
		for (size_t m = 0; m < this->server[n][LISTEN].size(); ++m)
		{
			if (unique.find(this->server[n][LISTEN][m]) == unique.end())
				unique[this->server[n][LISTEN][m]] = this->server[n][LISTEN][m];
			else if (n != 0)
			{
				this->server.erase(this->server.begin() + n);
				n--;
				break ;
			}
		}
	}
	for (size_t j = 0; j < this->server.size(); j++)
		for (size_t k = 0; k < this->server[j].vectorLocation.size(); k++)
			this->server[j].location[this->server[j].vectorLocation[k][LOCATION_READ_PATH][0]] = this->server[j].vectorLocation[k];
}

void	EuleeHand::perrorExit(std::string msg, int exitTrue)
{
	std::cerr << RED << msg << ": ";
	perror("");
	std::cerr << RESET;
	if (exitTrue)
		exit(EXIT_FAILURE);
}

long	EuleeHand::ft_select(int fd, void *buff, size_t size, Mode mode)
{
	fd_set	readFds, writeFds;
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);
	FD_SET(fd, (mode == READ) ? &readFds : &writeFds);

	timeval	timeout;
	timeout.tv_sec = WS_TIMEOUT;
	timeout.tv_usec = 0;

	int	ret = select(FD_SETSIZE, &readFds, &writeFds, NULL, &timeout);
	if (ret == -1)
	{
		this->perrorExit("Select Error", 0);
		return (-1);
	}
	else if (ret == 0)
	{
		std::cout << RED << "Select timeout!" << RESET << std::endl;
		return (0);
	}

	long	val = 0;
	if (FD_ISSET(fd, &readFds) && mode == READ)
	{
		val = recv(fd, buff, size, 0);
		if (val == -1)
			this->perrorExit("Read Error", 0);
	}
	else if (FD_ISSET(fd, &writeFds) && mode == WRITE)
	{
		val = send(fd, buff, size, 0);
		if (val == -1)
			this->perrorExit("Write Error", 0);
	}
	return (val);
}

int	EuleeHand::checkPath(std::string path, int isFile, int isDirectory)
{
    std::ifstream   temp(path + "/");
    if (temp.good() && isFile == 1 && isDirectory == 0)
        return (0);
	std::ifstream	file(path);
	if (file.good()) // is a directory and a file
	{
		if (path[path.length() - 1] == '/')
        {
            if (isDirectory)
                return (1);
            if (isFile)
                return (0);
        }
		std::ifstream	directory(path + "/");
        if (directory.good() && isDirectory) // directory
                return (1);
        if (isFile)
            return (1);
	}
	return (0);
}