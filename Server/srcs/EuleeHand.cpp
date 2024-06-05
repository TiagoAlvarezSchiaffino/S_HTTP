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
/*   Updated: 2024/06/04 14:23:14 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand() : envp(), cgi(), statusList(), buffer(), server(), serverFd(), serverAddr(), socket(), serverIndex(), useDefaultIndex(), method(), methodPath(), locationPath(), _envpSize() {}

EuleeHand::EuleeHand(std::string configFilePath, const ConfigManager &configManager, char **envp) : envp(), cgi(), statusList(), buffer(), server(), serverFd(), serverAddr(), socket(), serverIndex(), useDefaultIndex(), method(), methodPath(), locationPath(), _envpSize(), _configFilePath(configFilePath), _configManager(configManager)
{
	this->envp = new char*[100];
	for (size_t i = 0; envp[i]; ++i)
		this->addEnv(envp[i]);
}

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
		if (this->errorpage.size())
		{
			std::cout << ERROR_PAGE << " : ";
			for (std::map<int, std::string>::iterator it5 = errorpage.begin(); it5 != errorpage.end(); ++it5)
				std::cout << it5->first << " ";
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
		while (tokens[++j].token != ";")
			size++;
		j = i;
		while (tokens[++j].token != ";" && tokens[j].token != tokens[i + size].token)
		{
			if (tokens[j].token[0] != '.')
				this->_configManager.printError("cgi_index : invalid extension. ", j);
			else if (tokens[j].token[0] == '.')
			{
				std::string	temp = tokens[j].token.substr(1, tokens[j].token.length());
				for (size_t k = 0; k < temp.length(); ++k)
				{
					if (!isalpha(temp[k]) && temp[k] != '_')
						this->_configManager.printError("cgi_index : invalid extension. ", j);
				}
			}
		}
		if (tokens[j + 1].token == tokens[j + size].token)
			this->_configManager.printError("cgi_index : invalid arguments ", j);
		std::string	path = tokens[i + size].token;
		while (tokens[++i].token[0] == '.')
		{
			if (blockType == 1)
				this->cgi[tokens[i].token] = path;
			else
				location.cgi[tokens[i].token] = path;
		}
	}
	return (i);
}

bool	isNumeric(const std::string &str)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isdigit(*it)) {
			return (false);
		}
	}
    return (true);
}

size_t	EuleeHand::_parseErrorPage(std::vector<Token> &tokens, size_t i)
{
	if (tokens[i].token == "error_page" && tokens[i].type == KEY)
	{
		size_t	j = i;
		size_t	size = 0;
		while (tokens[++j].token != ";")
			size++;
		j = i;
		while (tokens[++j].token != ";" && tokens[j].token != tokens[i + size].token)
		{
			if (!isNumeric(tokens[j].token))
				this->_configManager.printError("error_page : invalid status code. ", j);
		}
		if (tokens[j + 1].token == tokens[j + size].token)
			this->_configManager.printError("error_page : invalid arguments ", j);
		std::string	path = tokens[i + size].token;
		while (isNumeric(tokens[++i].token))
			this->errorpage[std::atoi(tokens[i].token.c_str())] = path;
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
		i = this->_parsingHelper(tokens, i, loc, "error_page", ERROR_PAGE);
		i = this->_parsingHelper(tokens, i, loc, "autoindex", AUTO_INDEX);
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
		i = this->_parseErrorPage(tokens, i);
		i = this->_parseCgi(tokens, i, serv, 1);
		i = this->_parsingHelper(tokens, i, serv, "root", ROOT);
		i = this->_parsingHelper(tokens, i, serv, "index", INDEX);
		i = this->_parsingHelper(tokens, i, serv, "listen", LISTEN);
		i = this->_parsingHelper(tokens, i, serv, "return", RETURN);
		i = this->_parsingHelper(tokens, i, serv, "upload", UPLOAD);
		i = this->_parsingHelper(tokens, i, serv, "autoindex", AUTO_INDEX);
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

std::string EuleeHand::_getFileSize(const std::string &path, const std::string &file_name)
{
	std::string	full_path = path + "/" + file_name;
  	struct stat result;
	long		rv = stat(full_path.c_str(), &result);
    if (rv == 0)
    	return (std::to_string(result.st_size));
	else if (rv == -1)
    	std::cerr << "File {" <<  full_path << "} not found." << std::endl;
	return ("");
}

std::string EuleeHand::_getFileCreationTime(const std::string &path, const std::string &file_name)
{
	struct stat result;

	std::string	full_path = path + "/" + file_name;
	int rv = stat((full_path).c_str(), &result);
	if (rv == 0)
	{
    	std::time_t last_modified_time = result.st_mtime;   
    	std::time_t modified_time = static_cast<std::time_t>(last_modified_time);
		return (std::ctime(&modified_time));
    }
	else if (rv == -1)
    	std::cerr << "File {" <<  full_path << "} not found." << std::endl;
	return ("");
}

std::string EuleeHand::directoryListing(std::string path)
{
    DIR	*dir;
    struct dirent *ent;
	
	std::string lastMod;
	std::string fileSize;

    std::stringstream content;
    if ((dir = opendir(path.c_str())) != nullptr)
	{
        while ((ent = readdir(dir)) != nullptr)
		{
            std::string filename = ent->d_name;
			lastMod = this->_getFileCreationTime(path, filename);
			fileSize = this->_getFileSize(path, filename);
            if (filename != "." && filename != "..")
			{
                content << "<tr> <td><a href=\"" << filename << "\">" << filename << "</a></td>\n";
                content << "<td>" << lastMod << " </td>\n";
				content << "<td>" << fileSize << " bytes </td> </tr>\n";
			}
        }
		content << "</tbody></table></body></html>\n";
        closedir(dir);
    }
	std::stringstream head;
	std::stringstream style;
	std::stringstream body;
    std::stringstream html;
	
	head << "<html><head><title>I suck</title></head><body><h1>Directory shizer ";
	style << "<style>body {	font-family: Arial, sans-serif;	background-color: #f8f8f8;}h1 {	text-align: center;	font-size: 36px;	color: #333;	margin-top: 50px;}table {	margin: 0 auto;	border-collapse: collapse;	width: 80%;	max-width: 800px;	background-color: white;	box-shadow: 0px 2px 2px rgba(0, 0, 0, 0.1);}th {	font-size: 18px;	font-weight: bold;	background-color: #333;	color: white;	padding: 10px;	text-align: left;	border-bottom: 2px solid white;}td {	font-size: 16px;	padding: 10px;	text-align: left;	border-bottom: 1px solid #ccc;}tr:hover {	background-color: #f5f5f5;}a {	color: #333;	text-decoration: none;}a:hover {	color: #444;	text-decoration: underline;}</style></head>";
	body << " <body><h1>Index of " << path << "</h1><table><thead><tr><th>File Name</th><th>Last Modified</th><th>File Size</th></tr></thead><tbody>";

    html << head.str() << style.str() << body.str() << content.str();
    return (html.str());
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
	this->statusList[301] = "Moved Permanently";
	this->statusList[400] = "Bad Request";
	this->statusList[404] = "Not Found";
	this->statusList[405] = "Not Allowed";
	this->statusList[413] = "Request Entity Too Large";
	this->statusList[500] = "Internal Server Error";
}

void	EuleeHand::perrorExit(std::string msg, int exitTrue)
{
	std::cerr << RED << msg << ": ";
	perror("");
	std::cerr << RESET;
	if (exitTrue)
		exit(EXIT_FAILURE);
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
	return (this->cgi.find(extension) != this->cgi.end());
}

int	EuleeHand::checkExcept()
{
	if (this->server[this->serverIndex].location.find(this->methodPath) == this->server[this->serverIndex].location.end())
		return (0);
	int	found = 0;
	if (this->server[this->serverIndex].location[this->methodPath][LIMIT_EXCEPT].empty())
		return (0);
	for (size_t j = 0; j < this->server[this->serverIndex].location[this->methodPath][LIMIT_EXCEPT].size(); j++)
	{
		if (this->server[this->serverIndex].location[this->methodPath][LIMIT_EXCEPT][j] == this->method)
			found++;
	}
	if (found == 0)
	{
		this->sendHttp(405);
		return (1);
	}
	return (0);
}

void	EuleeHand::convertLocation()
{
	this->useDefaultIndex = 0;
	EuleePocket	myServer = this->server[this->serverIndex];
	std::string	methodPathCopy = this->methodPath.c_str();
	size_t		longestPathSize = 0;
	std::string	pathToFind, locationRoot, newPath, indexFile, remainingPath;
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
			if (this->checkPath(newPath, 1, 0) && newPath[newPath.length() - 1] != '/') // Found file, else found directory
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
	if (myServer.location[this->locationPath][INDEX].empty()) // No Trailing File -> Append back and find
	{
		remainingPath = this->methodPath.substr(this->locationPath.length());
		if (myServer[INDEX].empty() == false)
			indexFile = myServer[INDEX][0];
		this->methodPath = "/" + myServer[ROOT][0] + this->locationPath + (this->locationPath[this->locationPath.length() - 1] == '/' ? "" : "/") + (this->method == "GET" ? indexFile : ""); 
		this->useDefaultIndex = 1;
		this->useDirectoryListing = (this->server[this->serverIndex].location[this->locationPath][AUTO_INDEX].size() != 0);
	}
	else // Using Index
	{
		locationRoot = myServer.location[this->locationPath][ROOT][0];
		remainingPath = methodPathCopy.erase(0, this->locationPath.length());
		indexFile = myServer.location[this->locationPath][INDEX][0];
		this->methodPath = "/" + myServer.location[this->locationPath][ROOT][0] + remainingPath + ((remainingPath[remainingPath.length() - 1] == '/') ? "" : "/") + indexFile;
	}
	std::cout << GREEN << "Location Path: " << this->locationPath << RESET << std::endl;
	std::cout << GREEN << "New Path: " << this->methodPath << RESET << std::endl;
}

std::string	EuleeHand::extractHTML(std::string path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << RED << "Error: Could not open html" << RESET << std::endl;

		return ("");
	}
	std::string extract;
	std::string output;

	while (std::getline(file, output))
		extract = extract + output;
	return (extract);
}

int		EuleeHand::sendHttp(int statusCode, std::string htmlPath)
{
	if (this->statusList.find(statusCode) == this->statusList.end())
	{
		std::cerr << RED << "Cannot find status code!" << RESET << std::endl;
		std::cout << MAGENTA << "Returned " << statusCode << "!" << RESET << std::endl;
		return (statusCode);
	}
	std::string baseResponse = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusList[statusCode] + " \r\n\r\n";
	if (htmlPath.empty() && statusCode != 200)
		htmlPath = WS_ERROR_PAGE_PATH;
	else if (htmlPath.empty())
		htmlPath = WS_DEFAULT_PAGE_PATH;
	else
	{
		if (this->checkPath(htmlPath, 1, 0) == 0)
		{
			statusCode = 404;
			htmlPath = WS_ERROR_PAGE_PATH;
		}
	}
	
	std::string	htmlPage = extractHTML(htmlPath);
	if (htmlPage.empty() == false)
	{
		std::string code = "{{error_code}}";
		std::string msg = "{{error_message}}";

		baseResponse += htmlPage;
		if (statusCode != 200)
		{
			baseResponse.replace(baseResponse.find(code), code.length(), std::to_string(statusCode));	
			baseResponse.replace(baseResponse.find(code), code.length(), std::to_string(statusCode));	
			baseResponse.replace(baseResponse.find(msg), msg.length(), this->statusList[statusCode]);
		}
	}
	
	this->response[this->socket] = baseResponse;
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
	size_t	startPos = this->bufferTemp.find("\r\n\r\n") + std::strlen("\r\n\r\n");
	if (this->bufferTemp.length() - startPos > clientMaxBodySize)
	{
		std::cout << RED << "Client Body Size Exceeded!" << RESET << std::endl;
		this->sendHttp(413);
		return (1);
	}
	return (0);
}

size_t	EuleeHand::addEnv(std::string input)
{
	size_t	i = 0;
	std::string	temp = input.substr(0, input.find('='));
	for (; i < this->_envpSize; ++i)
	{
		std::string	str(this->envp[i]);
		str = str.substr(0, str.find('='));
		if (str == temp)
			break ;
	}
	if (i == this->_envpSize)
	{
		i = 0;
		this->envp[this->_envpSize] = new char[10000];
		std::memset(this->envp[this->_envpSize], 0, 10000);
		for (; input[i]; ++i)
			this->envp[this->_envpSize][i] = input[i];
		this->envp[this->_envpSize][i] = '\0';
		return (++this->_envpSize);
	}
	for (size_t j = 0; this->envp[i][j] != '\0'; ++j)
		this->envp[i][j] = '\0';
	for (size_t j = 0; input[j]; ++j)
		this->envp[i][j] = input[j];
	return (this->_envpSize);
}

int	EuleeHand::parseHeader()
{
	size_t	headerEndPos = this->buffer[this->socket].find("\r\n\r\n");
	if (headerEndPos == std::string::npos)
		return (0);
	std::istringstream	request(this->buffer[this->socket]);
	request >> this->method >> this->methodPath;

	size_t	transferEncoding = this->buffer[this->socket].find("Transfer-Encoding: chunked");
	if (transferEncoding != std::string::npos)
	{
		std::cout << (this->buffer[this->socket].find("\r\n0\r\n\r\n") != std::string::npos) << std::endl;
		return (this->buffer[this->socket].find("\r\n0\r\n\r\n") != std::string::npos);
	}

	size_t	contentLenghtPos = this->buffer[this->socket].find("Content-Length: ");
	if (contentLenghtPos != std::string::npos)
	{
		std::string contentLenghtStr = this->buffer[this->socket].substr(contentLenghtPos + std::strlen("Content-Length: "));
		size_t	contentLenght = std::stoul(contentLenghtStr.substr(0, contentLenghtStr.find("\r\n")));
		std::cout << "Content-Length: " << contentLenght << std::endl;
		std::string	messageBody = this->buffer[this->socket].substr(headerEndPos + std::strlen("\r\n\r\n"));
		if (messageBody.length() < (size_t)contentLenght)
			return (0);
	}
	return (1);
}