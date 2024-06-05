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
/*   Updated: 2024/06/05 10:56:50 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Serv.hpp"

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
		
		if (listen(this->_database.serverFd[i], WS_BACKLOG) < 0)
			this->_database.perrorExit("Listen Error");
	}
}

void	Serv::_acceptConnection()
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
}

int	Serv::_receiveRequest()
{
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
			std::cout << RED << "Receive Error: Connection interrupted!" << std::endl;
			close(this->_database.socket);
			return (1);
		}
		this->_database.buffer.append(readBuffer, valread);
		valread = this->_database.ft_select(this->_database.socket, readBuffer, WS_BUFFER_SIZE, READ);
	}
	return (0);
}

int	Serv::_handleFavicon()
{
	if (this->_database.methodPath != "/favicon.ico") // Ignore favicon
		return (0);
	std::string	message = "Go away favicon";
	std::cout << RED << message << RESET << std::endl;
	std::string response = "HTTP/1.1 404 Not Found\r\n\r\n" + message;
	this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.length(), WRITE);
	close(this->_database.socket);
	return (1);
}

void	Serv::_serverLoop()
{
	while(1)
	{
		this->_acceptConnection();

		if (this->_receiveRequest())
			continue ;
		if (this->_database.unchunkResponse())
			continue ;
		std::cout << GREEN << "Finished unchunking" << RESET << std::endl;

		std::istringstream	request(this->_database.buffer);
		request >> this->_database.method >> this->_database.methodPath;
		if (this->_handleFavicon())
			continue ;

		std::cout << BLUE << this->_database.buffer.substr(0, this->_database.buffer.find("\r\n\r\n")) << RESET << std::endl;
		// std::cout << BLUE << this->_database.buffer << RESET << std::endl;

		/* FOR DEBUGGING: TO DELETE */
		// if (this->_database.method == "GET")
		// {
		// 	std::cout << "Entered force output!" << std::endl;
		// 	std::string response = "HTTP/1.1 200 OK\r\n\r\n";
		// 	this->_database.ft_select(this->_database.socket, (void *)response.c_str(), response.size(), WRITE);
		// 	// this->_database.methodPath = "/cgi/srcs/cgi_static.cgi";
		// 	// HttpGetResponse	getResponse(this->_database);
		// 	// getResponse.handleGet();
		// 	close(this->_database.socket);
		// 	continue ;
		// }

		if (this->_database.checkExcept())
			continue ;
		this->_database.convertLocation();
		if (this->_database.checkClientBodySize())
			continue ;

		if (this->_database.method == "HEAD")
		{
			std::cout << MAGENTA << "Head method called" << RESET << std::endl;
			HttpHeadResponse	headResponse(this->_database);
			headResponse.handleHead();
		}
		// else if (this->_database.methodPath.find('.') != std::string::npos && (this->_database.method == "POST" && this->_database.methodPath == "/YoupiBanane/youpi.bla"))
		else if (this->_database.method == "POST" && this->_database.methodPath == "/YoupiBanane/youpi.bla")
		{
			std::cout << MAGENTA << "CGI method called" << RESET << std::endl;
			HttpCgiResponse	cgiResponse(this->_database);
			cgiResponse.handleCgi();
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
		else if (this->_database.method == "GET" && this->_database.methodPath != "/" && this->_database.isCGI() == 0) // Will be determined by the config
		{
			std::cout << MAGENTA << "Get method called" << RESET << std::endl;
			HttpGetResponse	getResponse(this->_database);
			getResponse.handleGet();
		}
		else
		{
			std::cout << MAGENTA << "Default method called" << RESET << std::endl;
			HttpDefaultResponse	defaultResponse(this->_database);
			defaultResponse.handleDefault();
		}
	}
}

// Upload to multiple directories