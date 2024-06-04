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
/*   Updated: 2024/06/04 11:22:36 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Serv.hpp"

/* Class constructor that takes in configFilePath string */
Serv::Serv(std::string configFilePath)
{
	this->_database = EuleeHand(configFilePath, ConfigManager(configFilePath));
	this->_configManager = ConfigManager(configFilePath);
	
	//Temporary to host 2 ports
	this->_serverAddr.resize(2);
	this->_serverFd.resize(2); 
}

Serv::~Serv(void) {}

void	Serv::_setupServer()
{
	addrinfo	hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = WS_DOMAIN;
	hints.ai_socktype = WS_TYPE;
	hints.ai_flags = WS_FLAGS;

	// Default port
	if ((this->_serverFd[0] = socket(WS_DOMAIN, WS_TYPE, WS_PROTOCOL)) < 0)
		this->_database.perrorExit("Socket Error");

	int	optval = 1;
	if (setsockopt(this->_serverFd[0], SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) == -1) //Done to keep socket alive even after Broken Pipe
		this->_database.perrorExit("Setsockopt Error");

	if (getaddrinfo(WS_SERVER_NAME, std::to_string(WS_PORT).c_str(), &hints, &res) != 0)
		this->_database.perrorExit("Getaddrinfo Error");
	
	memcpy(&this->_serverAddr[0], res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	this->_serverAddr[0].sin_port = htons(WS_PORT);

	if (bind(this->_serverFd[0], (sockaddr *)&this->_serverAddr[0], sizeof(this->_serverAddr[0])) < 0)
		this->_database.perrorExit("Bind Error");
	if (listen(this->_serverFd[0], WS_BACKLOG) < 0)
		this->_database.perrorExit("Listen Error");

	// Trying port 9090
	int	port = 9090;
	if ((this->_serverFd[1] = socket(WS_DOMAIN, WS_TYPE, WS_PROTOCOL)) < 0)
		this->_database.perrorExit("Socket Error");

	int	optval2 = 1;
	if (setsockopt(this->_serverFd[1], SOL_SOCKET, SO_NOSIGPIPE, &optval2, sizeof(optval)) == -1)
		this->_database.perrorExit("Setsockopt Error");

	if (getaddrinfo(WS_SERVER_NAME, std::to_string(port).c_str(), &hints, &res) != 0)
		this->_database.perrorExit("Getaddrinfo Error");
	
	memcpy(&this->_serverAddr[1], res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	this->_serverAddr[1].sin_port = htons(port);

	if (bind(this->_serverFd[1], (sockaddr *)&this->_serverAddr[1], sizeof(this->_serverAddr[1])) < 0)
		this->_database.perrorExit("Bind Error");
	if (listen(this->_serverFd[1], WS_BACKLOG) < 0)
		this->_database.perrorExit("Listen Error");
}

int	Serv::_unchunkResponse()
{
	std::string	header = this->_buffer.substr(0, this->_buffer.find("\r\n\r\n"));
	std::string	output;

	if (header.find("Transfer-Encoding: chunked") == std::string::npos)
		return (0);
	std::string	remaining = this->_buffer.substr(this->_buffer.find("\r\n\r\n") + 4);
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
	this->_buffer = header + "\r\n\r\n" + newBody;
	return (1);
}

void	Serv::_serverLoop()
{
	while(1)
	{
		std::cout << CYAN << "Port: " << WS_PORT << "\nWaiting for new connection..." << RESET << std::endl;
		this->_socket = 0;
		fcntl(this->_serverFd[0], F_SETFL, O_NONBLOCK);
		fcntl(this->_serverFd[1], F_SETFL, O_NONBLOCK);
		int i = 0;
		while (this->_socket <= 0)
		{
			for (i = 0; i < 2; i++)
			{
				this->_socket = accept(this->_serverFd[i], NULL, NULL);
				if (this->_socket != -1)
					break ;
			}
		}
		if (this->_socket < 0)
			this->_database.perrorExit("Accept Error");

		size_t		total = 0;
		char		readBuffer[WS_BUFFER_SIZE];
		this->_buffer.clear();
		long		valread = this->_database.ft_select(this->_socket, readBuffer, WS_BUFFER_SIZE, READ);
		while (valread > 0)
		{
			total += valread;
			std::cout << GREEN << "Received: " << valread << "\tTotal: " << total << RESET << std::endl;
			if (valread < 0)
			{
				close(this->_socket);
				return ;
			}
			this->_buffer.append(readBuffer, valread);
			valread = this->_database.ft_select(this->_socket, readBuffer, WS_BUFFER_SIZE, READ);
		}

		if (this->_unchunkResponse() == -1)
		{
			close(this->_socket);
			continue ;
		}

		std::string	method, query, contentType;
		std::istringstream	request(this->_buffer);
		
		request >> method >> this->_path;
		if (this->_path == "/favicon.ico") // Ignore favicon
		{
			std::string	message = "Go away favicon";
			std::cout << RED << message << RESET << std::endl;
			std::string response = "HTTP/1.1 404 Not Found\r\n\r\n" + message;

			this->_database.ft_select(this->_socket, (void *)response.c_str(), response.length(), WRITE);
			close(this->_socket);
			continue;
		}
		std::cout << BLUE << this->_buffer.substr(0, this->_buffer.find("\r\n\r\n")) << RESET << std::endl;

		if (method == "HEAD")
		{
			std::cout << "Head method called" << std::endl;
			HttpHeadResponse	headResponse(this->_socket, this->_path, this->_database);
			headResponse.handleHead();
		}
		else if (method == "POST")
		{
			std::cout << "Post method called" << std::endl;
			HttpPostResponse	postResponse(this->_socket, this->_buffer, this->_database);
			postResponse.handlePost();
		}
		else if (method == "DELETE")
		{
			std::cout << "Delete method called" << std::endl;
			HttpDeleteResponse	deleteResponse(this->_socket, this->_path, this->_database);
			deleteResponse.handleDelete();
		}
		else if (method == "GET" && this->_path != "/" && this->_path.find(".php") == std::string::npos && this->_path.find(".py") == std::string::npos && this->_path.find(".cgi") == std::string::npos) // Will be determined by the config
		{
			std::cout << "Get method called" << std::endl;
			HttpGetResponse	getResponse(this->_path, this->_socket, this->_database);
			getResponse.handleGet();
		}
		else if (this->_path.find('.') != std::string::npos)
		{
			std::cout << "CGI method called" << std::endl;
			HttpCgiResponse	cgiResponse(this->_path, method, this->_socket, this->_database);
			cgiResponse.handleCgi();
		}
		else
		{
			std::cout << "Default method called" << std::endl;
			HttpDefaultResponse	defaultResponse(this->_socket, this->_database);
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