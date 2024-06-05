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
/*   Updated: 2024/06/04 19:43:11 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand(void) : envp(), cgi(), server(), serverFd(), serverAddr(), methodPath(), buffer(), socket(), serverIndex(), useDefaultIndex(), _configFilePath(), _configManager() {}

EuleeHand::EuleeHand(std::string configFilePath, ConfigManager const &configManager) : envp(), cgi(), server(), serverFd(), serverAddr(), methodPath(), buffer(), socket(), serverIndex(), useDefaultIndex(), _configFilePath(configFilePath), _configManager(configManager) {}

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
				std::cout << it->second[k] << " ";
			std::cout << std::endl;
		}
		if (this->cgi.size())
		{
			std::cout << CGI << " : ";
			for (std::map<std::string, std::string>::iterator it3 = cgi.begin(); it3 != cgi.end(); ++it3)
				std::cout << it3->first << " ";
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
					std::cout << it2->second[d] << " ";
				std::cout << std::endl;
			}
			if (this->server[i].vectorLocation[j].cgi.size())
			{
				std::cout << CGI << " : ";
				for (std::map<std::string, std::string>::iterator it4 = this->server[i].vectorLocation[j].cgi.begin(); it4 != this->server[i].vectorLocation[j].cgi.end(); ++it4)
					std::cout << it4->first << " ";
				std::cout << std::endl;
			}
			if (j + 1 < this->server[i].vectorLocation.size())
				std::cout << std::endl;
		}
		std::cout << RESET;
		std::cout << "=======================" << std::endl;
	}
}

size_t	EuleeHand::_parseCgi(std::vector<Token> &tokens, size_t i, EuleeWallet &location, int blockType)
{
	if (tokens[i].token == "cgi_script" && tokens[i].type == KEY)
	{
		size_t	j = i;
		size_t	size = 0;
		std::string	path;
		while (tokens[++j].token != ";")
		{
			if (tokens[j].token.find("/") != std::string::npos || tokens[j].token.find(".") == std::string::npos)
			{
				if (++size > 1)
					this->_configManager.printError("cgi_index : too many paths. ", j);
				path = tokens[j].token;
			}
			else if (tokens[j].token[0] != '.')
				this->_configManager.printError("cgi_index : invalid extension. ", j);
			else if (tokens[j].token[0] == '.')
			{
				std::string	temp = tokens[j].token.substr(1, tokens[j].token.length());
				for (size_t k = 0; k < temp.length(); ++k)
				{
					if (!isalpha(temp[k]))
						this->_configManager.printError("cgi_index : invalid extension. ", j);
				}
			}
		}
		if (size == 0)
			this->_configManager.printError("cgi_index : no specified path. ", j);
		while (tokens[++i].token != ";")
		{
			if (tokens[i].token[0] == '.')
			{
				if (blockType == 1)
					this->cgi[tokens[i].token] = path;
				else
					location.cgi[tokens[i].token] = path;
			}
		}
	}
	return (i);
}

size_t	EuleeHand::_parsingHelper(std::vector<Token> &tokens, size_t i, EuleeWallet &location, std::string needle, Key key)
{
	if (tokens[i].token == needle && tokens[i].type == KEY)
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
		i = this->_parseCgi(tokens, i, loc, 2);
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
		i = this->_parseCgi(tokens, i, serv, 1);
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
				this->server.erase(this->server.begin() + n--);
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

int	EuleeHand::isCGI(void)
{
	size_t extensionPos = this->methodPath.find_last_of('.');
	if (extensionPos == std::string::npos)
		return (0);
	std::string extension = this->methodPath.substr(extensionPos);
	for (size_t i = 0; i < this->server[this->serverIndex][CGI].size(); i++)
		if (this->server[this->serverIndex][CGI][i] == extension)
			return (1);
	return (0);
}

int	EuleeHand::checkExcept(void)
{
	if (this->server[this->serverIndex].location.find(this->methodPath) == this->server[this->serverIndex].location.end())
		return (0);
	int	found = 0;
	if (this->server[this->serverIndex].location[this->methodPath][LIMIT_EXCEPT].size() == 0)
		return (0);
	for (size_t j = 0; j < this->server[this->serverIndex].location[this->methodPath][LIMIT_EXCEPT].size(); j++)
	{
		if (this->server[this->serverIndex].location[this->methodPath][LIMIT_EXCEPT][j] == this->method)
			found++;
	}
	if (found == 0)
	{
		std::string response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
		this->ft_select(this->socket, (void *)response.c_str(), response.size(), WRITE);
		close(this->socket);
		return (1);
	}
	return (0);
}

int	EuleeHand::unchunkResponse(void)
{
	std::string	output;
	std::string	header = this->buffer.substr(0, this->buffer.find("\r\n\r\n"));

	if (header.find("Transfer-Encoding: chunked") == std::string::npos)
		return (0);
	std::string	remaining = this->buffer.substr(this->buffer.find("\r\n\r\n") + 4);
	std::string	newBody = "";

	while (remaining.find("\r\n") != std::string::npos)
	{
		std::string	chunkSize = remaining.substr(0, remaining.find("\r\n"));
		size_t		size = std::stoul(chunkSize, 0, 16);
		if (size == 0)
			return (0);
		if (size > remaining.size() - std::strlen("\r\n"))
		{
			std::cout << RED << "Error: Chunk size is bigger than remaining size" << RESET << std::endl;
			return (-1);
		}
		newBody += remaining.substr(remaining.find("\r\n") + std::strlen("\r\n"), size);
		remaining = remaining.substr(remaining.find("\r\n") + size + std::strlen("\r\n\r\n"));
	}
	this->buffer = header + "\r\n\r\n" + newBody;
	return (1);
}

void	EuleeHand::convertLocation(void)
{
	/*
	 * Extract methodPath 
	 * strcmp each location path to method path to see whether it is a location or not
	 * -> If yes, check whether it has file trailing behind or not ....
	 * 		-> If yes, then we check whether it is file or directory
	 * 			-> If file, then we serve the file + 200 OK
	 * 			-> If directory, then do step below
	 * 		-> If no, then 404 Not Found
	 * -> If no, then we find whether it has index specified in the location block or not XXX
	 * 		-> If yes, then we append it back to methodPath and find
	 * 			-> If found, then we serve the file + 200 OK
	 * 			-> If not found, 404 Not found
	 * 		-> If no, then we go back to server block to find index
	 * 			-> If yes, then we append it back to methodPath and find
	 * 				-> If found, then we serve the file + 200 OK
	 * 				-> If not found, then 404 Not found
	 * 			-> If no, then 404 Not found
	 */

	this->useDefaultIndex = 0;
	EuleePocket	myServer = this->server[this->serverIndex];
	std::string	methodPathCopy = this->methodPath.c_str();
	size_t		longestPathSize = 0;
	std::string	locationPath, pathToFind, locationRoot, newPath, indexFile;
	for (std::map<std::string, EuleeWallet>::iterator it = myServer.location.begin(); it != myServer.location.end(); it++)
	{
		if (strncmp(it->first.c_str(), methodPathCopy.c_str(), it->first.length()) == 0 && it->first.length() > longestPathSize)
		{
			longestPathSize = it->first.length();
			locationPath = it->first;
		}
	}
	newPath = this->methodPath;
	if (methodPathCopy.length() - locationPath.length() > 1)
	{
		std::cout << "Trailing File" << std::endl;
		if (myServer.location[locationPath][ROOT].size() != 0)
		{
			locationRoot = myServer.location[locationPath][ROOT][0];
			newPath = locationRoot + methodPathCopy.substr(locationPath.length());
		}
		if (this->checkPath(newPath, 1, 1)) // Either file or directory
		{
			std::cout << "Found" << std::endl;
			if (this->checkPath(newPath, 1, 0)) // File
			{
				std::cout << "File" << std::endl;
				this->methodPath = "/" + newPath;
				std::cout << "Location Path: " << locationPath << std::endl;
				std::cout << GREEN << "New Path: " << this->methodPath << RESET << std::endl;
				return ;
			}
			else // Directory
				std::cout << "Directory" << std::endl;
		}
		else // Not Found
		{
			std::cout << "Not Found" << std::endl;
			return ;
		}
	}
	// 	-> If yes, then we append it back to methodPath and find
	//  	-> If found, then we serve the file + 200 OK
	//  	-> If not found, 404 Not found
	//  -> If no, then we go back to server block to find index
	//  	-> If yes, then we append it back to methodPath and find
	//  		-> If found, then we serve the file + 200 OK
	//  		-> If not found, then 404 Not found
	//  	-> If no, then 404 Not found
	std::cout << "No Trailing File" << std::endl;
	if (myServer.location[locationPath][INDEX].size() == 0)
	{
		std::cout << "Append back and find" << std::endl;
		indexFile = myServer[INDEX][0];
		this->methodPath = myServer[ROOT][0] + locationRoot + "/" + indexFile; 
		this->useDefaultIndex = 1;
	}
	else
	{
		std::cout << "Using index: " << newPath << std::endl;
		locationRoot = myServer.location[locationPath][ROOT][0];
		std::string	remainingPath = methodPathCopy.erase(0, locationPath.length());
		indexFile = myServer.location[locationPath][INDEX][0];
		this->methodPath = "/" + myServer.location[locationPath][ROOT][0] + remainingPath + "/" + indexFile;
	}
	std::cout << "Location Path: " << locationPath << std::endl;
	std::cout << GREEN << "New Path: " << this->methodPath << RESET << std::endl;
}

std::string	EuleeHand::cgiPath(void)
{
	if (this->methodPath.find_last_of(".") == std::string::npos)
		return ("");
	std::string	temp = this->methodPath.substr(this->methodPath.find_last_of("."));
	for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); ++it)
	{
		if (temp == it->first)
			return (it->second);
	}
	return ("");
}