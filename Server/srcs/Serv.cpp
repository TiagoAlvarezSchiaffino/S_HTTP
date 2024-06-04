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
/*   Updated: 2024/06/04 13:04:49 by Tiago                  /_____/ U         */
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

	
	// Default port
	for (size_t i = 0; i < this->_database.server.size(); i++)
	{
		if ((this->_database.serverFd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			this->_database.perrorExit("Socket Error");

		int	optval = 1;
		if (setsockopt(this->_database.serverFd[i], SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1) //Done to keep socket alive even after Broken Pipe
			this->_database.perrorExit("Setsockopt Error");

		if (getaddrinfo(this->_database.server[i][SERVER_NAME][0].c_str(), this->_database.server[i][LISTEN][0].c_str(), &hints, &res) != 0)
			this->_database.perrorExit("Getaddrinfo Error");
		
		memcpy(&this->_database.serverAddr[i], res->ai_addr, res->ai_addrlen);
		freeaddrinfo(res);
		this->_database.serverAddr[i].sin_port = htons(std::stoi(this->_database.server[i][LISTEN][0]));

		if (bind(this->_database.serverFd[i], (sockaddr *)&this->_database.serverAddr[i], sizeof(this->_database.serverAddr[i])) < 0)
			this->_database.perrorExit("Bind Error");
		if (listen(this->_database.serverFd[i], WS_BACKLOG) < 0)
			this->_database.perrorExit("Listen Error");
	}


	// for (size_t i = 0; i < this->_database.server.size(); i++)
	// {
	// 	if ((this->_database.serverFd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	// 		this->_database.perrorExit("Socket Error");

	// 	int	optval = 1;
	// 	if (setsockopt(this->_database.serverFd[i], SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1) //Done to keep socket alive even after Broken Pipe
	// 		this->_database.perrorExit("Setsockopt Error");

	// 	if (getaddrinfo(this->_database.server[i][SERVER_NAME][0].c_str(), this->_database.server[i][LISTEN][0].c_str(), &hints, &res) != 0)
	// 		this->_database.perrorExit("Getaddrinfo Error");
		
	// 	memcpy(&this->_database.serverAddr[i], res->ai_addr, res->ai_addrlen);
	// 	freeaddrinfo(res);
	// 	this->_database.serverAddr[i].sin_port = htons(std::stoi(this->_database.server[i][LISTEN][0]));

	// 	if (bind(this->_database.serverFd[i], (sockaddr *)&this->_database.serverAddr[i], sizeof(this->_database.serverAddr[i])) < 0)
	// 		this->_database.perrorExit("Bind Error");
	// 	if (listen(this->_database.serverFd[i], WS_BACKLOG) < 0)
	// 		this->_database.perrorExit("Listen Error");
	// }

	// Trying port 9090
	// int	port = 9090;
	// if ((this->_serverFd[1] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	// 	this->_database.perrorExit("Socket Error");

	// int	optval2 = 1;
	// if (setsockopt(this->_serverFd[1], SOL_SOCKET, SO_NOSIGPIPE, &optval2, sizeof(optval)) == -1)
	// 	this->_database.perrorExit("Setsockopt Error");

	// if (getaddrinfo(WS_SERVER_NAME, std::to_string(port).c_str(), &hints, &res) != 0)
	// 	this->_database.perrorExit("Getaddrinfo Error");
	
	// memcpy(&this->_serverAddr[1], res->ai_addr, res->ai_addrlen);
	// freeaddrinfo(res);
	// this->_serverAddr[1].sin_port = htons(port);

	// if (bind(this->_serverFd[1], (sockaddr *)&this->_serverAddr[1], sizeof(this->_serverAddr[1])) < 0)
	// 	this->_database.perrorExit("Bind Error");
	// if (listen(this->_serverFd[1], WS_BACKLOG) < 0)
	// 	this->_database.perrorExit("Listen Error");
}

int	Serv::_unchunkResponse()
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
		std::cout << size << std::endl;
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

void	Serv::_serverLoop()
{
	while(1)
	{
		std::cout << CYAN << "Port Accepted: ";
		for (size_t i = 0; i < this->_database.server.size(); i++)
			std::cout << this->_database.server[i][LISTEN][0] << " ";
		std::cout << "Waiting for new connection..." << RESET << std::endl;
		this->_database.socket = 0;
		for (size_t i = 0; i < this->_database.server.size(); i++)
			fcntl(this->_database.serverFd[i], F_SETFL, O_NONBLOCK);
		while (this->_database.socket <= 0)
		{
			for (size_t i = 0; i < this->_database.server.size(); i++)
			{
				this->_database.socket = accept(this->_database.serverFd[i], NULL, NULL);
				if (this->_database.socket != -1)
					break ;
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
			// std::cout << GREEN << "Received: " << valread << "\tTotal: " << total << RESET << std::endl;
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

		std::string			method;
		std::istringstream	request(this->_database.buffer);
		
		request >> method >> this->_database.methodPath;
		if (this->_database.methodPath == "/favicon.ico") // Ignore favicon
		{
			std::string	message = "Go away favicon";
			std::cout << RED << message << RESET << std::endl;
			std::string response = "HTTP/1.1 404 Not Found\r\n\r\n" + message;

			this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.length(), WRITE);
			close(this->_database.socket);
			continue;
		}
		std::cout << BLUE << this->_database.buffer.substr(0, this->_database.buffer.find("\r\n\r\n")) << RESET << std::endl;
		// std::cout << BLUE << this->_database.buffer << RESET << std::endl;

		if (method == "HEAD")
		{
			std::cout << MAGENTA << "Head method called" << RESET << std::endl;
			HttpHeadResponse	headResponse(this->_database);
			headResponse.handleHead();
		}
		else if (method == "POST")
		{
			std::cout << MAGENTA << "Post method called" << RESET << std::endl;
			HttpPostResponse	postResponse(this->_database);
			postResponse.handlePost();
		}
		else if (method == "PUT")
		{
			std::cout << MAGENTA << "Put method called" << RESET << std::endl;
			HttpPutResponse	putResponse(this->_database);
			putResponse.handlePut();
		}
		else if (method == "DELETE")
		{
			std::cout << MAGENTA << "Delete method called" << RESET << std::endl;
			HttpDeleteResponse	deleteResponse(this->_database);
			deleteResponse.handleDelete();
		}
		else if (method == "GET" && this->_database.methodPath != "/" && this->_database.methodPath.find(".php") == std::string::npos && this->_database.methodPath.find(".py") == std::string::npos && this->_database.methodPath.find(".cgi") == std::string::npos) // Will be determined by the config
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
			std::cout << "Default method called" << std::endl;
			HttpDefaultResponse	defaultResponse(this->_database);
			defaultResponse.handleDefault();
		}
	}
}

void	Serv::runServer(void)
{
	this->_database.parseConfigFile();
	// this->_database.printTokens();
	std::cout << GREEN "Config File Parsing Done..." RESET << std::endl;
	// this->_database.configLibrary();
	// this->_database.errorHandleShit();
	std::cout << GREEN "Error Handling File Done..." RESET << std::endl;
	this->_database.parseConfigServer();
	this->_database.printServers();
	std::cout << GREEN "Config Server Parsing Done..." RESET << std::endl;
	this->_setupServer();
	this->_serverLoop();
}