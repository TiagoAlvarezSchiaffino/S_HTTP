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
/*   Updated: 2024/06/05 10:47:00 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand() : envp(), cgi(), statusList(), server(), serverFd(), serverAddr(), socket(), serverIndex(), useDefaultIndex(), method(), methodPath(), buffer(), locationPath() {}

EuleeHand::EuleeHand(std::string configFilePath, const ConfigManager &configManager, char **envp) : envp(envp), cgi(), statusList(), server(), serverFd(), serverAddr(), socket(), serverIndex(), useDefaultIndex(), method(), methodPath(), buffer(), locationPath(), _configFilePath(configFilePath), _configManager(configManager) {}

EuleeHand::~EuleeHand() {}

void	EuleeHand::printTokens()
{
	this->_configManager.printTokens();
}

void	EuleeHand::parseConfigFile()
{
	this->_configManager.parseConfigFile();
}

void	EuleeHand::configLibrary()
{
	this->_configManager.configLibrary();
}

void	EuleeHand::errorHandleShit()
{
	this->_configManager.errorHandleShit();
}

void	EuleeHand::printServers()
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

		// i = this->_parseCgi(tokens, i);
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

void	EuleeHand::parseConfigServer()
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
	this->statusList[200] = "OK";
	this->statusList[404] = "Not Found";
	this->statusList[405] = "Not Allowed";
	this->statusList[413] = "Request Entity Too Large";
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
	size_t	total = 0;
	if (FD_ISSET(fd, &readFds) && mode == READ)
	{
		val = recv(fd, buff, size, 0);
		if (val == -1)
			this->perrorExit("Read Error", 0);
		return (val);
	}
	else if (FD_ISSET(fd, &writeFds) && mode == WRITE)
	{
		val = send(fd, (char *)buff, size, 0);
		if (val == (long)size)
			return (val);
		while (val > 0)
		{
			total += val;
			std::cout << GREEN << "Sent: " << val << "\tTotal: " << total << RESET << std::endl;
			val = this->ft_select(fd, (char *)buff + total, size - total, WRITE);
			if (val == -1)
				this->perrorExit("Write Error", 0);
		}
	}
	return (total);
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

int	EuleeHand::isCGI()
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

int	EuleeHand::checkExcept()
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

int	EuleeHand::unchunkResponse()
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
			close(this->socket);
			return (1);
		}
		newBody += remaining.substr(remaining.find("\r\n") + std::strlen("\r\n"), size);
		remaining = remaining.substr(remaining.find("\r\n") + size + std::strlen("\r\n\r\n"));
	}
	this->buffer = header + "\r\n\r\n" + newBody;
	return (0);
}

void	EuleeHand::convertLocation()
{
	this->useDefaultIndex = 0;
	EuleePocket	myServer = this->server[this->serverIndex];
	std::string	methodPathCopy = this->methodPath.c_str();
	size_t		longestPathSize = 0;
	std::string	pathToFind, locationRoot, newPath, indexFile;
	for (std::map<std::string, EuleeWallet>::iterator it = myServer.location.begin(); it != myServer.location.end(); it++)
	{
		if (strncmp(it->first.c_str(), methodPathCopy.c_str(), it->first.length()) == 0 && it->first.length() > longestPathSize)
		{
			longestPathSize = it->first.length();
			this->locationPath = it->first;
		}
	}
	newPath = this->methodPath;
	if (methodPathCopy.length() - this->locationPath.length() > 1) // Trailing File
	{
		if (myServer.location[this->locationPath][ROOT].size() != 0)
		{
			locationRoot = myServer.location[this->locationPath][ROOT][0];
			newPath = locationRoot + methodPathCopy.substr(this->locationPath.length());
		}
		if (this->checkPath(newPath, 1, 1)) // Either file or directory
		{
			if (this->checkPath(newPath, 1, 0)) // Found file, else found directory
			{
				this->methodPath = "/" + newPath;
				std::cout << GREEN << "Location Path: " << this->locationPath << RESET << std::endl;
				std::cout << GREEN << "New Path: " << this->methodPath << RESET << std::endl;
				return ;
			}
		}
		else // Not Found
			return ;
	}
	if (myServer.location[this->locationPath][INDEX].size() == 0) // No Trailing File -> Append back and find
	{
		this->methodPath = myServer[ROOT][0] + locationRoot + "/" + indexFile; 
		this->useDefaultIndex = 1;
	}
	else // Using Index
	{
		locationRoot = myServer.location[this->locationPath][ROOT][0];
		std::string	remainingPath = methodPathCopy.erase(0, this->locationPath.length());
		indexFile = myServer.location[this->locationPath][INDEX][0];
		this->methodPath = "/" + myServer.location[this->locationPath][ROOT][0] + remainingPath + "/" + indexFile;
	}
	std::cout << GREEN << "Location Path: " << this->locationPath << RESET << std::endl;
	std::cout << GREEN << "New Path: " << this->methodPath << RESET << std::endl;
}

std::string	EuleeHand::extractHTML(std::string path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file" << std::endl;
		exit(1);
	}
	std::string extract;
	std::string output;

	while (std::getline(file, output))
		extract = extract + output;
	return (extract);
}

int		EuleeHand::sendHttp(int statusCode, int closeSocket, std::string htmlPath)
{
	if (this->statusList.find(statusCode) == this->statusList.end())
	{
		std::cerr << RED << "Cannot find status code!" << RESET << std::endl;
		std::cout << MAGENTA << "Returned " << statusCode << "!" << RESET << std::endl;
		return (statusCode);
	}
	std::string response = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusList[statusCode] + " \r\n\r\n";
	if (htmlPath.size() == 0 && statusCode != 200)
		htmlPath =  "./html/error.html";
	else if (htmlPath.size() == 0)
		htmlPath =  "./html/index.html";
	else
	{
		if (this->checkPath(htmlPath, 1, 0) == 0)
		{
			statusCode = 404;
			htmlPath =  "./html/error.html";
		}
	}
	response += extractHTML(htmlPath);
	std::string code = "{{error_code}}";
	std::string msg = "{{error_message}}";

	if (statusCode != 200)
	{
		response.replace(response.find(code), code.length(), std::to_string(statusCode));	
		response.replace(response.find(code), code.length(), std::to_string(statusCode));	
		response.replace(response.find(msg), msg.length(), this->statusList[statusCode]);
	}
	ft_select(this->socket, (void *)response.c_str(), response.length(), WRITE);
	if (closeSocket)
		close (this->socket);
	std::cout << MAGENTA << "Returned " << statusCode << "!" << RESET << std::endl;
	return (statusCode);
}

std::string	EuleeHand::cgiPath()
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

int	EuleeHand::checkClientBodySize()
{
	size_t	clientMaxBodySize = std::numeric_limits<std::size_t>::max();
	if (this->server[this->serverIndex][CLIENT_MAX_BODY_SIZE].size() != 0)
		clientMaxBodySize = std::stoul(this->server[this->serverIndex][CLIENT_MAX_BODY_SIZE][0]);
	if (this->server[this->serverIndex].location[this->locationPath][CLIENT_MAX_BODY_SIZE].size() != 0)
		clientMaxBodySize = std::min(clientMaxBodySize, std::stoul(this->server[this->serverIndex].location[this->locationPath][CLIENT_MAX_BODY_SIZE][0]));
	std::cout << clientMaxBodySize << std::endl;
	size_t	startPos = this->buffer.find("\r\n\r\n") + std::strlen("\r\n\r\n");
	std::cout << this->buffer << std::endl;
	std::cout << startPos << std::endl;
	std::cout << this->buffer.length() - startPos << std::endl;
	if (this->buffer.length() - startPos > clientMaxBodySize)
		return (this->sendHttp(413, 1));
	return (0);
}