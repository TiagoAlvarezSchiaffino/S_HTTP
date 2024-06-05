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
/*   Updated: 2024/06/04 15:33:40 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Serv.hpp"

fd_set	readFds_cpy, writeFds_cpy;

Serv::Serv(std::string configFilePath, char **envp)
{
	this->_database = EuleeHand(configFilePath, ConfigManager(configFilePath), envp);
	this->_configManager = ConfigManager(configFilePath);
}

Serv::~Serv() {}

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
	this->_database.addEnv("SERVER_PROTOCOL=HTTP/1.1");
	this->_database.addEnv("HTTP_X_SECRET_HEADER_FOR_TEST=1");
	this->_setupServer();
	this->_serverLoop();
}

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
		if (setsockopt(this->_database.serverFd[i], SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1)
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
		
		if (listen(this->_database.serverFd[i], SOMAXCONN) < 0)
			this->_database.perrorExit("Listen Error");
	}
}

void	Serv::_acceptConnection()
{
	this->_database.socket = accept(this->_database.serverFd[0], NULL, NULL);
	if (this->_database.socket == -1)
	{
		this->_database.perrorExit("Accept Error", 0);
		close(this->_database.socket);
	}
	// fcntl(this->_database.socket, F_SETFL, O_NONBLOCK);
	FD_SET(this->_database.socket, &this->_database.myReadFds);
	std::cout << GREEN << "New connection accepted!" << RESET << std::endl;
}

void	Serv::_receiveRequest()
{
	char	readBuffer[WS_BUFFER_SIZE];

	long	recvVal = recv(this->_database.socket, readBuffer, WS_BUFFER_SIZE, 0);
	while (recvVal > 0)
	{
		this->_database.buffer[this->_database.socket].append(readBuffer, recvVal);
		std::cout << GREEN << "Total: " << this->_database.buffer[this->_database.socket].size() << RESET << std::endl;
		recvVal = recv(this->_database.socket, readBuffer, WS_BUFFER_SIZE, 0);
		if (recvVal < 0)
			break ;
	}
	if (this->_database.parseHeader())
	{
		FD_SET(this->_database.socket, &this->_database.myWriteFds);
		FD_CLR(this->_database.socket, &this->_database.myReadFds);	
	}
}

int	Serv::_handleFavicon()
{
	if (this->_database.methodPath != "/favicon.ico") // Ignore favicon
		return (0);
	std::cout << RED << "Go away favicon" << RESET << std::endl;
	this->_database.sendHttp(404);
	return (1);
}

int	Serv::_handleRedirection()
{
	if (this->_database.server[this->_database.serverIndex].location.find(this->_database.methodPath) == this->_database.server[this->_database.serverIndex].location.end())
		return (0);
	if (this->_database.server[this->_database.serverIndex].location[this->_database.methodPath][RETURN].empty())
		return (0);
	std::string	statusCode = this->_database.server[this->_database.serverIndex].location[this->_database.methodPath][RETURN][0];
	std::string	redirectionPath = this->_database.server[this->_database.serverIndex].location[this->_database.methodPath][RETURN][1];
	std::string response = "HTTP/1.1 " + statusCode + " " + this->_database.statusList[std::stoi(statusCode)] + "\r\nLocation: " + redirectionPath + "\r\n\r\n";
	this->_database.response[this->_database.socket] = response;
	std::cout << GREEN << "Redirected with status code " + statusCode + " to: " + redirectionPath << RESET << std::endl;
	return (1);
}

int	Serv::_parseRequest()
{
	// if (this->_database.unchunkResponse())
		// continue ;
	std::cout << GREEN << "Finished unchunking" << RESET << std::endl;

	std::istringstream	request(this->_database.buffer[this->_database.socket]);
	request >> this->_database.method >> this->_database.methodPath;
	if (this->_handleFavicon())
		return (1);

	// std::cout << BLUE << this->_database.buffer[this->_database.socket].substr(0, this->_database.buffer[this->_database.socket].find("\r\n\r\n")) << RESET << std::endl;

	if (this->_handleRedirection())
		return (1) ;
	if (this->_database.checkExcept())
		return (1) ;
	this->_database.convertLocation();
	if (this->_database.checkClientBodySize())
		return (1) ;

	this->_database.addEnv("REQUEST_METHOD=" + this->_database.method);
	return (0);
}

void	Serv::_doRequest()
{
	try
	{
		if (this->_database.isCGI())
		{
			std::cout << MAGENTA << "CGI method called" << RESET << std::endl;
			HttpCgiResponse	cgiResponse(&this->_database);
			cgiResponse.handleCgi();
		}
		else if (this->_database.method == "HEAD")
		{
			std::cout << MAGENTA << "Head method called" << RESET << std::endl;
			HttpHeadResponse	headResponse(&this->_database);
			headResponse.handleHead();
		}
		else if (this->_database.method == "POST")
		{
			std::cout << MAGENTA << "Post method called" << RESET << std::endl;
			HttpPostResponse	postResponse(&this->_database);
			postResponse.handlePost();
		}
		else if (this->_database.method == "PUT")
		{
			std::cout << MAGENTA << "Put method called" << RESET << std::endl;
			HttpPutResponse	putResponse(&this->_database);
			putResponse.handlePut();
		}
		else if (this->_database.method == "DELETE")
		{
			std::cout << MAGENTA << "Delete method called" << RESET << std::endl;
			HttpDeleteResponse	deleteResponse(&this->_database);
			deleteResponse.handleDelete();
		}
		else if (this->_database.method == "GET")
		{
			std::cout << MAGENTA << "Get method called" << RESET << std::endl;
			HttpGetResponse	getResponse(&this->_database);
			getResponse.handleGet();
		}
	}
	catch(const std::exception& e)
	{
		std::cout << RED << e.what() << RESET << '\n';
		std::remove(WS_TEMP_FILE_IN);
		std::remove(WS_TEMP_FILE_OUT);
		this->_database.sendHttp(500);
	}
}

void	Serv::_writeResponse()
{
	std::cout << GREEN << "Writing!" << RESET << std::endl;
	int sendVal = send(this->_database.socket, this->_database.response[this->_database.socket].c_str(), this->_database.response[this->_database.socket].size(), 0);
	if (sendVal < 0)
		this->_database.perrorExit("Send Error", 0);
	this->_database.buffer[this->_database.socket].clear();
	this->_database.response[this->_database.socket].clear();
	std::cout << "Sent finished!" << std::endl;
	close(this->_database.socket);
}

void	Serv::_serverLoop()
{
	fd_set	readFds, writeFds;
	timeval	timeout;
	
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	FD_ZERO(&this->_database.myReadFds);
	FD_ZERO(&this->_database.myWriteFds);
	for (size_t i = 0; i < this->_database.serverFd.size(); i++)
	{
		FD_SET(this->_database.serverFd[i], &this->_database.myReadFds);
		fcntl(this->_database.serverFd[i], F_SETFL, O_NONBLOCK);
	}
	this->_database.socket = 0;
	std::cout << CYAN << "Port Accepted: ";
	for (size_t i = 0; i < this->_database.server.size(); i++)
		std::cout << this->_database.server[i][LISTEN][this->_database.server[i].portIndex] << " ";
	std::cout << RESET << std::endl;
	while (1)
	{
		readFds = this->_database.myReadFds;
		writeFds = this->_database.myWriteFds;
		int	selectVal = select(FD_SETSIZE, &readFds, &writeFds, NULL, &timeout);
		if (selectVal == 0)
			continue ;
		for (int fd = 3; fd < FD_SETSIZE; fd++)
		{
			if (!FD_ISSET(fd, &readFds))
				continue ;
			if (fd == this->_database.serverFd[0])
				this->_acceptConnection();
			else
			{
				this->_database.socket = fd;
				this->_receiveRequest();
			}
		}
		for (int fd = 3; fd < FD_SETSIZE; fd++)
		{
			if (!FD_ISSET(fd, &writeFds))
				continue ;
			if (this->_parseRequest() == 0)
				this->_doRequest();
			this->_writeResponse();


			this->_database.socket = fd;
			FD_CLR(fd, &this->_database.myWriteFds);
		}
	}
}

// Upload to multiple directories