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
/*   Updated: 2024/06/06 05:04:26 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand() : envp(), cgi(), statusList(), buffer(), server(), serverFd(), serverAddr(), socket(), _envpSize() {}

EuleeHand::EuleeHand(std::string configFilePath, const ConfigManager &configManager, char **envp) : envp(), cgi(), statusList(), buffer(), server(), serverFd(), serverAddr(), socket(), _envpSize(), _configFilePath(configFilePath), _configManager(configManager)
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
	size_t extensionPos = this->methodPath[this->socket].find_last_of('.');
	if (extensionPos == std::string::npos)
		return (0);
	std::string extension = this->methodPath[this->socket].substr(extensionPos);
	return (this->cgi.find(extension) != this->cgi.end());
}

int	EuleeHand::checkExcept()
{
	if (this->server[this->serverIndex[this->socket]].location.find(this->methodPath[this->socket]) == this->server[this->serverIndex[this->socket]].location.end())
		return (0);
	int	found = 0;
	if (this->server[this->serverIndex[this->socket]].location[this->methodPath[this->socket]][LIMIT_EXCEPT].empty())
		return (0);
	for (size_t j = 0; j < this->server[this->serverIndex[this->socket]].location[this->methodPath[this->socket]][LIMIT_EXCEPT].size(); j++)
	{
		if (this->server[this->serverIndex[this->socket]].location[this->methodPath[this->socket]][LIMIT_EXCEPT][j] == this->method[this->socket])
			found++;
	}
	if (found == 0)
	{
		this->sendHttp(405);
		return (1);
	}
	return (0);
}

size_t	EuleeHand::_readFile(std::string *buffer1, std::string *buffer2, int infile, char *temp, long bytes_read, int type, int *count)
{
	size_t	current_size = 0;

	if (*count > type)
		buffer2->append(temp, bytes_read);
	else
		{
		if (buffer1->find("\r\n") != buffer1->length() - 2)
		{
			std::string	str(temp);
			size_t	pos = str.find("\r\n");
			while (pos == std::string::npos)
			{
				*buffer1 += str;
				std::memset(temp, 0, WS_BUFFER_SIZE);
				bytes_read = read(infile, temp, WS_BUFFER_SIZE);
				current_size += bytes_read;
				std::string	next(temp);
				str = next;
				pos = str.find("\r\n");
			}
			std::string	before = str.substr(0, pos) + "\r\n";
			std::string	after = str.substr(pos + 2);
			*buffer1 += before;
			*buffer2 += after;
		}
		*count = *count + 1;
	}
	return (current_size);
}

int	EuleeHand::_unchunkIntofile(int fd, std::string bufferIn, int isHeader)
{
	if (bufferIn == "")
		return (1);
	size_t		pos = bufferIn.find("\r\n\r\n");
	std::string	remaining = bufferIn;
	if (isHeader)
	{
		remaining = bufferIn.substr(pos + 4);
		std::string	header = bufferIn.substr(0, pos) + "\r\n\r\n";
		write(fd, header.c_str(), header.size());
	}

	while (1)
	{
		size_t	size = 0;
		pos = remaining.find("\r\n");
		if (pos == std::string::npos)
			break ;
		std::string	chunkSize = remaining.substr(0, pos);
		try
		{
			size = std::stoul(chunkSize, 0, 16);
		}
		catch(const std::exception& e)
		{
			std::cout << "Chunk Error: Hex size is less than chunk size!" << std::endl;
		}
		if (size == 0)
			break ;
		if (size > remaining.size() - std::strlen("\r\n"))
			std::cout << "Chunk Error: Hex size is more than remaining size!" << std::endl;
		std::string	tmp = remaining.substr(pos + std::strlen("\r\n"), size);
		write(fd, tmp.c_str(), tmp.size());
		remaining = remaining.substr(pos + size + std::strlen("\r\n\r\n"));
	}
	return (0);
}

int	EuleeHand::unchunkResponse()
{
	if (this->buffer[this->socket].find("Transfer-Encoding: chunked") == std::string::npos)
		return (0);
	std::vector<std::string>	bufferVector(10, "");
	long		bytes_read = 0;
	int			infile = open(WS_UNCHUNK_INFILE, O_RDWR | O_CREAT | O_TRUNC, 0777);
	write(infile, this->buffer[this->socket].c_str(), this->buffer[this->socket].size());
	close(infile);

	infile = open(WS_UNCHUNK_INFILE, O_RDONLY, 0777);
	char		*temp = new char[WS_BUFFER_SIZE + 1];
    std::memset(temp, 0, WS_BUFFER_SIZE + 1);

	std::ifstream	countSize(WS_UNCHUNK_INFILE);
	countSize.seekg(0, std::ios::end);
	size_t	total = countSize.tellg();
	countSize.seekg(0, std::ios::beg);
	countSize.close();
	size_t	current_size = 0;
	infile = open(WS_UNCHUNK_INFILE, O_RDONLY, 0777);
	if (total <= 25000000)
	{
		while ((bytes_read = read(infile, temp, WS_BUFFER_SIZE)) > 0)
		{
			bufferVector[0].append(temp, bytes_read);
			std::memset(temp, 0, WS_BUFFER_SIZE + 1);
		}
	}
	else
	{
		int	count = 0;
		while ((bytes_read = read(infile, temp, WS_BUFFER_SIZE)) > 0)
		{
			current_size += bytes_read;
			if (current_size <= total / 10)
				bufferVector[0].append(temp, bytes_read);
			else if (current_size > total / 10 && current_size <= total * 2 / 10)
				current_size += this->_readFile(&bufferVector[0], &bufferVector[1], infile, temp, bytes_read, 0, &count);
			else if (current_size > total * 2 / 10 && current_size <= total * 3 / 10)
				current_size += this->_readFile(&bufferVector[1], &bufferVector[2], infile, temp, bytes_read, 1, &count);
			else if (current_size > total * 3 / 10 && current_size <= total * 4 / 10)
				current_size += this->_readFile(&bufferVector[2], &bufferVector[3], infile, temp, bytes_read, 2, &count);
			else if (current_size > total * 4 / 10 && current_size <= total * 5 / 10)
				current_size += this->_readFile(&bufferVector[3], &bufferVector[4], infile, temp, bytes_read, 3, &count);
			else if (current_size > total * 5 / 10 && current_size <= total * 6 / 10)
				current_size += this->_readFile(&bufferVector[4], &bufferVector[5], infile, temp, bytes_read, 4, &count);
			else if (current_size > total * 6 / 10 && current_size <= total * 7 / 10)
				current_size += this->_readFile(&bufferVector[5], &bufferVector[6], infile, temp, bytes_read, 5, &count);
			else if (current_size > total * 7 / 10 && current_size <= total * 8 / 10)
				current_size += this->_readFile(&bufferVector[6], &bufferVector[7], infile, temp, bytes_read, 6, &count);
			else if (current_size > total * 8 / 10 && current_size <= total * 9 / 10)
				current_size += this->_readFile(&bufferVector[7], &bufferVector[8], infile, temp, bytes_read, 7, &count);
			else
				current_size += this->_readFile(&bufferVector[8], &bufferVector[9], infile, temp, bytes_read, 8, &count);
			std::memset(temp, 0, WS_BUFFER_SIZE + 1);
		}
	}
	close(infile);

	int	outfile = open(WS_UNCHUNK_OUTFILE, O_CREAT | O_TRUNC | O_RDWR, 0777);
	for (size_t i = 0; i < bufferVector.size(); i++)
	{
		this->_unchunkIntofile(outfile, bufferVector[i], (i == 0));
		std::cout << MAGENTA << "Unchunking: " << (i + 1 / bufferVector.size()) + 1 << "0%" << RESET << "\r";
		std::cout.flush();
	}
	std::cout << std::endl;
	close(outfile);

	infile = open(WS_UNCHUNK_OUTFILE, O_RDONLY, 0777);
	this->buffer[this->socket].clear();
	while ((bytes_read = read(infile, temp, WS_BUFFER_SIZE)) > 0)
	{
		this->buffer[this->socket].append(temp, bytes_read);
		std::memset(temp, 0, WS_BUFFER_SIZE + 1);
	}
	close(infile);
	std::remove(WS_UNCHUNK_INFILE);
	std::remove(WS_UNCHUNK_OUTFILE);
	delete[] temp;
	return (0);
}

void	EuleeHand::convertLocation()
{
	this->useDefaultIndex[this->socket] = 0;
	EuleePocket	myServer = this->server[this->serverIndex[this->socket]];
	std::string	methodPathCopy = this->methodPath[this->socket].c_str();
	size_t		longestPathSize = 0;
	std::string	pathToFind, locationRoot, newPath, indexFile, remainingPath;
	for (std::map<std::string, EuleeWallet>::iterator it = myServer.location.begin(); it != myServer.location.end(); it++)
	{
		if (strncmp(it->first.c_str(), methodPathCopy.c_str(), it->first.length()) == 0 && it->first.length() > longestPathSize)
		{
			longestPathSize = it->first.length();
			this->locationPath[this->socket] = it->first;
		}
	}
	newPath = this->methodPath[this->socket];
	if (methodPathCopy.length() - this->locationPath[this->socket].length() > 1) // Trailing File
	{
		if (myServer.location[this->locationPath[this->socket]][ROOT].size() != 0)
		{
			locationRoot = myServer.location[this->locationPath[this->socket]][ROOT][0];
			newPath = locationRoot + methodPathCopy.substr(this->locationPath[this->socket].length());
		}
		if (this->checkPath(newPath, 1, 1)) // Either file or directory
		{
			if (this->checkPath(newPath, 1, 0) && newPath[newPath.length() - 1] != '/') // Found file, else found directory
			{
				this->methodPath[this->socket] = "/" + newPath;
				std::cout << GREEN << "Location Path: " << this->locationPath[this->socket] << RESET << std::endl;
				std::cout << GREEN << "New Path: " << this->methodPath[this->socket] << RESET << std::endl;
				return ;
			}
		}
		else // Not Found
			return ;
	}
	if (myServer.location[this->locationPath[this->socket]][INDEX].empty()) // No Trailing File -> Append back and find
	{
		remainingPath = this->methodPath[this->socket].substr(this->locationPath[this->socket].length());
		if (myServer[INDEX].empty() == false)
			indexFile = myServer[INDEX][0];
		this->methodPath[this->socket] = "/" + myServer[ROOT][0] + this->locationPath[this->socket] + (this->locationPath[this->socket][this->locationPath[this->socket].length() - 1] == '/' ? "" : "/") + (this->method[this->socket] == "GET" ? indexFile : ""); 
		this->useDefaultIndex[this->socket] = 1;
		this->useDirectoryListing[this->socket] = (this->server[this->serverIndex[this->socket]].location[this->locationPath[this->socket]][AUTO_INDEX].size() != 0);
	}
	else // Using Index
	{
		locationRoot = myServer.location[this->locationPath[this->socket]][ROOT][0];
		remainingPath = methodPathCopy.erase(0, this->locationPath[this->socket].length());
		indexFile = myServer.location[this->locationPath[this->socket]][INDEX][0];
		this->methodPath[this->socket] = "/" + myServer.location[this->locationPath[this->socket]][ROOT][0] + remainingPath + ((remainingPath[remainingPath.length() - 1] == '/') ? "" : "/") + indexFile;
	}
	std::cout << GREEN << "Location Path: " << this->locationPath[this->socket] << RESET << std::endl;
	std::cout << GREEN << "New Path: " << this->methodPath[this->socket] << RESET << std::endl;
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

int		EuleeHand::sendHttp(int statusCode, std::string responseBody)
{
	std::string baseResponse = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusList[statusCode] + " \r\n\r\n";
	if (responseBody.empty() == false)
	{
		this->response[this->socket] = baseResponse + responseBody;
		return (statusCode);
	}

	if (this->errorpage.find(statusCode) != this->errorpage.end())
	{
		if (this->checkPath(this->errorpage[statusCode], 1, 0) == 0)
		{
			std::cerr << RED << "Error page is not found! Default is used. " << RESET <<std::endl;

			std::string code = "{{error_code}}";
			std::string msg = "{{error_message}}";
			baseResponse += extractHTML(WS_ERROR_PAGE_PATH);
		
			baseResponse.replace(baseResponse.find(code), code.length(), std::to_string(statusCode));	
			baseResponse.replace(baseResponse.find(code), code.length(), std::to_string(statusCode));	
			baseResponse.replace(baseResponse.find(msg), msg.length(), this->statusList[statusCode]);
		}
		else
		{
			std::cout << GREEN << "Error HTML page is found!" << RESET << std::endl;
			baseResponse += extractHTML(this->errorpage[statusCode]); // use client page
		}
	}
	else
	{
		if (this->checkPath(this->errorpage[statusCode], 1, 0) == 0)
			baseResponse += extractHTML(WS_DEFAULT_PAGE_PATH);
		else
			baseResponse += extractHTML(methodPath[this->socket]);

	}
	this->response[this->socket] = baseResponse;
	std::cout << MAGENTA << "Returned " << statusCode << "!" << RESET << std::endl;
	return (statusCode);
}

std::string	EuleeHand::cgiPath()
{
	if (this->methodPath[this->socket].find_last_of(".") == std::string::npos)
		return ("");
	std::string	temp = this->methodPath[this->socket].substr(this->methodPath[this->socket].find_last_of("."));
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
	if (this->server[this->serverIndex[this->socket]][CLIENT_MAX_BODY_SIZE].size() != 0)
		clientMaxBodySize = std::stoul(this->server[this->serverIndex[this->socket]][CLIENT_MAX_BODY_SIZE][0]);
	if (this->server[this->serverIndex[this->socket]].location[this->locationPath[this->socket]][CLIENT_MAX_BODY_SIZE].size() != 0)
		clientMaxBodySize = std::min(clientMaxBodySize, std::stoul(this->server[this->serverIndex[this->socket]].location[this->locationPath[this->socket]][CLIENT_MAX_BODY_SIZE][0]));
	size_t	startPos = this->buffer[this->socket].find("\r\n\r\n") + std::strlen("\r\n\r\n");
	if (this->buffer[this->socket].length() - startPos > clientMaxBodySize)
	{
		std::cerr << RED << "Client Body Size Exceeded!" << RESET << std::endl;
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
	request >> this->method[this->socket] >> this->methodPath[this->socket];

	size_t	transferEncoding = this->buffer[this->socket].find("Transfer-Encoding: chunked");
	if (transferEncoding != std::string::npos)
		return (this->buffer[this->socket].find("\r\n0\r\n\r\n") != std::string::npos);

	size_t	contentLenghtPos = this->buffer[this->socket].find("Content-Length: ");
	if (contentLenghtPos != std::string::npos)
	{
		std::string contentLenghtStr = this->buffer[this->socket].substr(contentLenghtPos + std::strlen("Content-Length: "));
		size_t	contentLenght = std::stoul(contentLenghtStr.substr(0, contentLenghtStr.find("\r\n")));
		std::string	messageBody = this->buffer[this->socket].substr(headerEndPos + std::strlen("\r\n\r\n"));
		return (messageBody.length() >= (size_t)contentLenght);
	}
	return (1);
}