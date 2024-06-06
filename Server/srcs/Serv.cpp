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
/*   Updated: 2024/06/06 04:46:20 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Serv.hpp"

Serv::Serv(std::string configFilePath)
{
	this->_database = EuleeHand(configFilePath, ConfigManager(configFilePath));
	std::remove(WS_TEMP_FILE_IN);
	std::remove(WS_TEMP_FILE_OUT);
	std::remove(WS_UNCHUNK_INFILE);
	std::remove(WS_UNCHUNK_OUTFILE);
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

void	Serv::_acceptConnection(int fd)
{
	this->_database.socket = accept(fd, NULL, NULL);
	if (this->_database.socket == -1)
		this->_database.perrorExit("Accept Error", 0);
	fcntl(this->_database.socket, F_SETFL, O_NONBLOCK);
	for (size_t i = 0; i < this->_database.serverFd.size(); i++)
	{
		if (fd == this->_database.serverFd[i])
		{
			this->_database.serverIndex[this->_database.socket] = i;
			break ;
		}
	}
	this->_database.bytes_sent[this->_database.socket] = 0;
	this->_database.parsed[this->_database.socket] = false;
	this->_database.connectionCount++;
	FD_SET(this->_database.socket, &this->_database.myReadFds);
}

void	Serv::_receiveRequest()
{
	char	readBuffer[WS_BUFFER_SIZE + 1];

	std::memset(readBuffer, 0, WS_BUFFER_SIZE + 1);
	long	recvVal = recv(this->_database.socket, readBuffer, WS_BUFFER_SIZE, 0);
	while (recvVal > 0)
	{
		this->_database.buffer[this->_database.socket].append(readBuffer, recvVal);
		std::cout << GREEN << "Receiving total: " << this->_database.buffer[this->_database.socket].size() << "\r" << RESET;
		std::cout.flush();
		std::memset(readBuffer, 0, WS_BUFFER_SIZE + 1);
		recvVal = recv(this->_database.socket, readBuffer, WS_BUFFER_SIZE, 0);
		if (recvVal < 0)
			break ;
	}
	if (this->_database.parseHeader())
	{
		std::cout << std::endl;
		FD_SET(this->_database.socket, &this->_database.myWriteFds);
		FD_CLR(this->_database.socket, &this->_database.myReadFds);
	}
}

void	Serv::_sendResponse()
{
	static int countOut = 0;
	long	total = this->_database.bytes_sent[this->_database.socket];
	long	sendVal = send(this->_database.socket, this->_database.response[this->_database.socket].c_str() + total, this->_database.response[this->_database.socket].size() - total, 0);
	if (sendVal < 0)
	{
		this->_database.perrorExit("Send Error", 0);
		this->_database.bytes_sent[this->_database.socket] = 0;
		this->_database.buffer[this->_database.socket].clear();
		this->_database.response[this->_database.socket].clear();
		this->_database.parsed.erase(this->_database.socket);
		close(this->_database.socket);
		FD_CLR(this->_database.socket, &this->_database.myWriteFds);
		return ;
	}
	this->_database.bytes_sent[this->_database.socket] += sendVal;
	std::cout << GREEN << "Sending total: " << this->_database.bytes_sent[this->_database.socket] << RESET << "\r";
	std::cout.flush();

	if ((size_t)this->_database.bytes_sent[this->_database.socket] != this->_database.response[this->_database.socket].size())
		return ;

	std::cout << std::endl;
	this->_database.bytes_sent.erase(this->_database.socket);
	this->_database.buffer.erase(this->_database.socket);
	this->_database.response.erase(this->_database.socket);
	this->_database.parsed.erase(this->_database.socket);

	close(this->_database.socket);
	FD_CLR(this->_database.socket, &this->_database.myWriteFds);
	std::cout << YELLOW << "Replied back to " << ++countOut << " connections!" << std::endl;
	std::cerr << "Closed " << this->_database.socket << std::endl;
  
	std::cout << CYAN << "Port Accepted: ";
	for (size_t i = 0; i < this->_database.server.size(); i++)
		std::cout << this->_database.server[i][LISTEN][this->_database.server[i].portIndex] << " ";
	std::cout << "\nWaiting for new connections..." << RESET << std::endl;
}

int	Serv::_handleFavicon()
{
	if (this->_database.methodPath[this->_database.socket] != "/favicon.ico")
		return (0);
	std::cerr << RED << "Go away favicon" << RESET << std::endl;
	this->_database.sendHttp(404);
	return (1);
}

int	Serv::_handleRedirection()
{
	if (this->_database.server[this->_database.serverIndex[this->_database.socket]].location.find(this->_database.methodPath[this->_database.socket]) == this->_database.server[this->_database.serverIndex[this->_database.socket]].location.end())
		return (0);
	if (this->_database.server[this->_database.serverIndex[this->_database.socket]].location[this->_database.methodPath[this->_database.socket]][RETURN].empty())
		return (0);
	std::string	statusCode = this->_database.server[this->_database.serverIndex[this->_database.socket]].location[this->_database.methodPath[this->_database.socket]][RETURN][0];
	std::string	redirectionPath = this->_database.server[this->_database.serverIndex[this->_database.socket]].location[this->_database.methodPath[this->_database.socket]][RETURN][1];
	std::string response = "HTTP/1.1 " + statusCode + " " + this->_database.statusList[std::stoi(statusCode)] + "\r\nLocation: " + redirectionPath + "\r\n\r\n";
	this->_database.response[this->_database.socket] = response;
	std::cout << GREEN << "Redirected with status code " + statusCode + " to: " + redirectionPath << RESET << std::endl;
	return (1);
}

int	Serv::_parseRequest()
{
	if (this->_database.unchunkResponse())
		return (1);
	std::cout << GREEN << "Finished unchunking" << RESET << std::endl;
	std::istringstream	request(this->_database.buffer[this->_database.socket]);
	request >> this->_database.method[this->_database.socket] >> this->_database.methodPath[this->_database.socket];
	if (this->_handleFavicon())
		return (1);

	std::cout << BLUE << this->_database.buffer[this->_database.socket].substr(0, this->_database.buffer[this->_database.socket].find("\r\n\r\n")) << RESET << std::endl;

	if (this->_handleRedirection())
		return (1) ;
	if (this->_database.checkExcept())
		return (1) ;
	this->_database.convertLocation();
	if (this->_database.checkClientBodySize())
		return (1) ;
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
		else if (this->_database.method[this->_database.socket] == "HEAD")
		{
			std::cout << MAGENTA << "Head method called" << RESET << std::endl;
			HttpHeadResponse	headResponse(&this->_database);
			headResponse.handleHead();
		}
		else if (this->_database.method[this->_database.socket] == "POST")
		{
			std::cout << MAGENTA << "Post method called" << RESET << std::endl;
			HttpPostResponse	postResponse(&this->_database);
			postResponse.handlePost();
		}
		else if (this->_database.method[this->_database.socket] == "PUT")
		{
			std::cout << MAGENTA << "Put method called" << RESET << std::endl;
			HttpPutResponse	putResponse(&this->_database);
			putResponse.handlePut();
		}
		else if (this->_database.method[this->_database.socket] == "DELETE")
		{
			std::cout << MAGENTA << "Delete method called" << RESET << std::endl;
			HttpDeleteResponse	deleteResponse(&this->_database);
			deleteResponse.handleDelete();
		}
		else if (this->_database.method[this->_database.socket] == "GET")
		{
			std::cout << MAGENTA << "Get method called" << RESET << std::endl;
			HttpGetResponse	getResponse(&this->_database);
			getResponse.handleGet();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << '\n';
		std::remove(WS_TEMP_FILE_IN);
		std::remove(WS_TEMP_FILE_OUT);
		this->_database.sendHttp(500);
	}
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
		this->_database.parsed[this->_database.serverFd[i]] = false;
		FD_SET(this->_database.serverFd[i], &this->_database.myReadFds);
	}
	this->_database.socket = 0;
	std::cout << CYAN << "Port Accepted: ";
	for (size_t i = 0; i < this->_database.server.size(); i++)
		std::cout << this->_database.server[i][LISTEN][this->_database.server[i].portIndex] << " ";
	std::cout << "\nWaiting for new connections..." << RESET << std::endl;
	static int countIn = 0;
	while (1)
	{
		usleep(2000);
		memcpy(&readFds, &this->_database.myReadFds, sizeof(this->_database.myReadFds));
		memcpy(&writeFds, &this->_database.myWriteFds, sizeof(this->_database.myWriteFds));
		int	selectVal = select(FD_SETSIZE, &readFds, &writeFds, NULL, &timeout);
		if (selectVal == 0)
			continue ;
		for (int fd = 0; fd < FD_SETSIZE; fd++) // Reading
		{
			if (!FD_ISSET(fd, &readFds))
				continue ;
			int	isServerFd = 0;
			for (size_t i = 0; i < this->_database.serverFd.size(); i++)
				isServerFd += (fd == this->_database.serverFd[i]);
			if (isServerFd)
			{
				this->_acceptConnection(fd);
				std::cout << YELLOW << "Accepted " << ++countIn << " connections!" << std::endl;
			}
			else
			{
				this->_database.socket = fd;
				this->_receiveRequest();
			}
		}
		for (int fd = 0; fd < FD_SETSIZE; fd++) // Writing
		{
			if (!FD_ISSET(fd, &writeFds))
				continue ;
			this->_database.socket = fd;
			int	oneIsParsed = 0;
			for (size_t i = 0; i < FD_SETSIZE; i++)
				oneIsParsed += this->_database.parsed[i];
			if (this->_database.parsed[this->_database.socket] == false && oneIsParsed == 0)
			{
				if (this->_parseRequest() == 0)
					this->_doRequest();
				this->_database.parsed[this->_database.socket] = true;
			}
			if (this->_database.parsed[this->_database.socket])
				this->_sendResponse();
		}
	}
}