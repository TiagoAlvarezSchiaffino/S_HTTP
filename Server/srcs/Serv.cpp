/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Serv.cpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:54:16 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 16:42:11 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Serv.hpp"

/* Class constructor that takes in configFilePath string */
Serv::Serv(std::string configFilePath)
{
	this->_database = EuleeHand(configFilePath, ConfigManager(configFilePath));
	this->_configManager = ConfigManager(configFilePath);
}

Serv::~Serv(void) {}

void	Serv::_setupServer()
{
	addrinfo	hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	this->_database.serverAddr.resize(this->_database.server.size());
	this->_database.serverFd.resize(this->_database.server.size()); 
	
	for (size_t i = 0; i < this->_database.server.size(); i++)
	{
		if ((this->_database.serverFd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			this->_database.perrorExit("Socket Error");

		int	optval = 1;
		if (setsockopt(this->_database.serverFd[i], SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1) //Done to keep socket alive even after Broken Pipe
			this->_database.perrorExit("Setsockopt Error");

		this->_database.server[i][SERVER_NAME].push_back("localhost");
		this->_database.server[i][LISTEN].push_back("80");
		for (size_t j = 0; j < this->_database.server[i][LISTEN].size() && this->_database.server[i].portIndex == -1; j++)
		{
			if (getaddrinfo(this->_database.server[i][SERVER_NAME][0].c_str(), this->_database.server[i][LISTEN][j].c_str(), &hints, &res) != 0)
				this->_database.perrorExit("Getaddrinfo Error");

			memcpy(&this->_database.serverAddr[i], res->ai_addr, res->ai_addrlen);
			freeaddrinfo(res);
			this->_database.serverAddr[i].sin_port = htons(std::stoi(this->_database.server[i][LISTEN][j]));

			if (bind(this->_database.serverFd[i], (sockaddr *)&this->_database.serverAddr[i], sizeof(this->_database.serverAddr[i])) < 0)
				continue ;
			else
				this->_database.server[i].portIndex = j;
		}
		if (this->_database.server[i].portIndex == -1)
			this->_database.perrorExit("Bind Error");
		
		if (listen(this->_database.serverFd[i], WS_BACKLOG) < 0)
			this->_database.perrorExit("Listen Error");
	}
}

int	Serv::_unchunkResponse() // Util
{
	std::string	header = this->_database.buffer.substr(0, this->_database.buffer.find("\r\n\r\n"));
	std::string	output;

	if (header.find("Transfer-Encoding: chunked") == std::string::npos)
		return (0);
	std::string	remaining = this->_database.buffer.substr(this->_database.buffer.find("\r\n\r\n") + 4);
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
	this->_database.buffer = header + "\r\n\r\n" + newBody;
	return (1);
}

int	Serv::_checkExcept() // Util
{
	if (this->_database.server[this->_database.serverIndex].location.find(this->_database.methodPath) == this->_database.server[this->_database.serverIndex].location.end())
		return (0);
	int	found = 0;
	if (this->_database.server[this->_database.serverIndex].location[this->_database.methodPath][LIMIT_EXCEPT].size() == 0)
		return (0);
	for (size_t j = 0; j < this->_database.server[this->_database.serverIndex].location[this->_database.methodPath][LIMIT_EXCEPT].size(); j++)
	{
		if (this->_database.server[this->_database.serverIndex].location[this->_database.methodPath][LIMIT_EXCEPT][j] == this->_database.method)
			found++;
	}
	if (found == 0)
	{
		std::string response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
		this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.size(), WRITE);
		close(this->_database.socket);
		return (1);
	}
	return (0);
}

int		Serv::_isCGI() // Util
{
	size_t extensionPos = this->_database.methodPath.find_last_of('.');
	if (extensionPos == std::string::npos)
		return (0);
	std::string extension = this->_database.methodPath.substr(extensionPos);
	for (size_t i = 0; i < this->_database.server[this->_database.serverIndex][CGI].size(); i++)
		if (this->_database.server[this->_database.serverIndex][CGI][i] == extension)
			return (1);
	return (0);
}

void	Serv::_convertLocation() // Util
{
	/**
	 * XExtract methodPath 
	 * Xstrcmp each location path to method path to see whether it is a location or not
	 * X-> If yes, check whether it has file trailing behind or not ....
	 * X	-> If yes, then we check whether it is file or directory
	 * X		-> If file, then we serve the file + 200 OK
	 * 			-> If directory, then do step below
	 * X	-> If no, then 404 Not Found
	 * X-> If no, then we find whether it has index specified in the location block or not XXX
	 * 		-> If yes, then we append it back to methodPath and find
	 * 			-> If found, then we serve the file + 200 OK
	 * 			-> If not found, 404 Not found
	 * 		-> If no, then we go back to server block to find index
	 * 			-> If yes, then we append it back to methodPath and find
	 * 				-> If found, then we serve the file + 200 OK
	 * 				-> If not found, then 404 Not found
	 * 			-> If no, then 404 Not found
	 */

	this->_database.useDefaultIndex = 0;
	EuleePocket	myServer = this->_database.server[this->_database.serverIndex];
	std::string	methodPathCopy = this->_database.methodPath.c_str();
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
	newPath = this->_database.methodPath;
	if (methodPathCopy.length() - locationPath.length() > 1)
	{
		std::cout << "Trailing File" << std::endl;
		if (myServer.location[locationPath][ROOT].size() != 0)
		{
			locationRoot = myServer.location[locationPath][ROOT][0];
			newPath = locationRoot + methodPathCopy.substr(locationPath.length());
		}
		if (this->_database.checkPath(newPath, 1, 1)) // Either file or directory
		{
			std::cout << "Found" << std::endl;
			if (this->_database.checkPath(newPath, 1, 0)) // File
			{
				std::cout << "File" << std::endl;
				this->_database.methodPath = "/" + newPath;
				std::cout << "Location Path: " << locationPath << std::endl;
				std::cout << GREEN << "New Path: " << this->_database.methodPath << RESET << std::endl;
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
	// 	* 		-> If yes, then we append it back to methodPath and find
	//  * 			-> If found, then we serve the file + 200 OK
	//  * 			-> If not found, 404 Not found
	//  * 		-> If no, then we go back to server block to find index
	//  * 			-> If yes, then we append it back to methodPath and find
	//  * 				-> If found, then we serve the file + 200 OK
	//  * 				-> If not found, then 404 Not found
	//  * 			-> If no, then 404 Not found
	std::cout << "No Trailing File" << std::endl;
	if (myServer.location[locationPath][INDEX].size() == 0)
	{
		std::cout << "Append back and find" << std::endl;
		indexFile = myServer[INDEX][0];
		this->_database.methodPath = myServer[ROOT][0] + locationRoot + "/" + indexFile; 
		this->_database.useDefaultIndex = 1;
	}
	else
	{
		std::cout << "Using index: " << newPath << std::endl;
		locationRoot = myServer.location[locationPath][ROOT][0];
		std::string	remainingPath = methodPathCopy.erase(0, locationPath.length());
		indexFile = myServer.location[locationPath][INDEX][0];
		this->_database.methodPath = "/" + myServer.location[locationPath][ROOT][0] + remainingPath + "/" + indexFile;
	}
	std::cout << "Location Path: " << locationPath << std::endl;
	std::cout << GREEN << "New Path: " << this->_database.methodPath << RESET << std::endl;
}

void	Serv::_serverLoop()
{
	while(1)
	{
		std::cout << CYAN << "Port Accepted: ";
		for (size_t i = 0; i < this->_database.server.size(); i++)
			std::cout << this->_database.server[i][LISTEN][this->_database.server[i].portIndex] << " ";
		std::cout << "\nWaiting for new connection..." << RESET << std::endl;
		this->_database.socket = 0;
		for (size_t i = 0; i < this->_database.server.size(); i++)
			fcntl(this->_database.serverFd[i], F_SETFL, O_NONBLOCK);
		while (this->_database.socket <= 0)
		{
			for (size_t i = 0; i < this->_database.server.size(); i++)
			{
				this->_database.socket = accept(this->_database.serverFd[i], NULL, NULL);
				if (this->_database.socket != -1)
				{
					this->_database.serverIndex = i;
					break ;
				}
			}
		}
		if (this->_database.socket < 0)
			this->_database.perrorExit("Accept Error");

		size_t		total = 0;
		char		readBuffer[WS_BUFFER_SIZE];
		this->_database.buffer.clear();
		long		valread = this->_database.ft_select(this->_database.socket, readBuffer, WS_BUFFER_SIZE, READ);
		while (valread > 0)
		{
			total += valread;
			std::cout << GREEN << "Received: " << valread << "\tTotal: " << total << RESET << std::endl;
			if (valread < 0)
			{
				close(this->_database.socket);
				return ;
			}
			this->_database.buffer.append(readBuffer, valread);
			valread = this->_database.ft_select(this->_database.socket, readBuffer, WS_BUFFER_SIZE, READ);
		}

		if (this->_unchunkResponse() == -1)
		{
			close(this->_database.socket);
			continue ;
		}

		std::istringstream	request(this->_database.buffer);
		
		request >> this->_database.method >> this->_database.methodPath;
		if (this->_database.methodPath == "/favicon.ico") // Ignore favicon
		{
			std::string	message = "Go away favicon";
			std::cout << RED << message << RESET << std::endl;
			std::string response = "HTTP/1.1 404 Not Found\r\n\r\n" + message;

			this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.length(), WRITE);
			close(this->_database.socket);
			continue;
		}
		// std::cout << BLUE << this->_database.buffer.substr(0, this->_database.buffer.find("\r\n\r\n")) << RESET << std::endl;
		std::cout << BLUE << this->_database.buffer << RESET << std::endl;

		if (this->_checkExcept())
			continue ;
		this->_convertLocation();

		if (this->_database.method == "HEAD")
		{
			std::cout << MAGENTA << "Head method called" << RESET << std::endl;
			HttpHeadResponse	headResponse(this->_database);
			headResponse.handleHead();
		}
		else if (this->_database.method == "POST")
		{
			std::cout << MAGENTA << "Post method called" << RESET << std::endl;
			HttpPostResponse	postResponse(this->_database);
			postResponse.handlePost();
		}
		else if (this->_database.method == "PUT")
		{
			std::cout << MAGENTA << "Put method called" << RESET << std::endl;
			HttpPutResponse	putResponse(this->_database);
			putResponse.handlePut();
		}
		else if (this->_database.method == "DELETE")
		{
			std::cout << MAGENTA << "Delete method called" << RESET << std::endl;
			HttpDeleteResponse	deleteResponse(this->_database);
			deleteResponse.handleDelete();
		}
		else if (this->_database.method == "GET" && this->_database.methodPath != "/" && this->_isCGI() == 0) // Will be determined by the config
		{
			std::cout << MAGENTA << "Get method called" << RESET << std::endl;
			HttpGetResponse	getResponse(this->_database);
			getResponse.handleGet();
		}
		else if (this->_database.methodPath.find('.') != std::string::npos)
		{
			std::cout << MAGENTA << "CGI method called" << RESET << std::endl;
			HttpCgiResponse	cgiResponse(this->_database);
			cgiResponse.handleCgi();
		}
		else
		{
			std::cout << MAGENTA << "Default method called" << RESET << std::endl;
			HttpDefaultResponse	defaultResponse(this->_database);
			defaultResponse.handleDefault();
		}
	}
}

void	Serv::runServer()
{
	this->_database.parseConfigFile();
	std::cout << GREEN "Config File Parsing Done..." RESET << std::endl;
	this->_database.configLibrary();
	this->_database.errorHandleShit();
	std::cout << GREEN "Error Handling File Done..." RESET << std::endl;
	this->_database.parseConfigServer();
	this->_database.printServers();
	std::cout << GREEN "Config Server Parsing Done..." RESET << std::endl;
	this->_setupServer();
	this->_serverLoop();
}