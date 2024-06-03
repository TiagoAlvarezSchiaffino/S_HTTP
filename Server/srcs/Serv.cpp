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
/*   Updated: 2024/06/03 15:37:23 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Serv.hpp"

Serv::Serv(std::string configFilePath): _configFilePath(configFilePath)
{
	this->_configManager = ConfigManager(configFilePath);
}

Serv::~Serv() {}

void	Serv::_perrorExit(std::string msg)
{
	std::cerr << RED << msg << ": ";
	perror("");
	std::cerr << RESET;
	exit(EXIT_FAILURE);
}

void	Serv::_setupServer()
{
	struct addrinfo	hints, *res;

	if ((this->_serverFd = socket(WS_DOMAIN, WS_TYPE, WS_PROTOCOL)) < 0)
		this->_perrorExit("Socket Error");
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = WS_DOMAIN;
	hints.ai_socktype = WS_TYPE;
	hints.ai_flags = WS_FLAGS;

	if (getaddrinfo(WS_SERVER_NAME, std::to_string(WS_PORT).c_str(), &hints, &res) != 0)
		this->_perrorExit("Getaddrinfo Error");
	
	memcpy(&this->_serverAddr, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	this->_serverAddr.sin_port = htons(WS_PORT);

	if (bind(this->_serverFd, (struct sockaddr *)&this->_serverAddr, sizeof(this->_serverAddr)) < 0)
		this->_perrorExit("Bind Error");
	if (listen(this->_serverFd, WS_BACKLOG) < 0)
		this->_perrorExit("Listen Error");

	this->_fds[0].fd = this->_serverFd;
	this->_fds[0].events = POLLIN;
	this->_ret = poll(this->_fds, 1, WS_TIMEOUT);
}

static std::string	get_content_type(std::string file)
{
	std::string	extension = file.substr(file.find_last_of(".") + 1);

	if (extension == "jpg" || extension == "jpeg" || extension == "png")
		return ("Content-Type: image/" + extension + "\r\n");
	if (extension == "html" || extension == "css" || extension == "js")
		return ("Content-Type: text/" + ((extension == "js") ? "javascript" : extension) + "\r\n");
	if (extension == "mp3")
		return ("Content-Type: audio/mpeg\r\n");
	if (extension == "mp4")
		return ("Content-Type: video/mp4\r\n");
	if (extension == "json" || extension == "pdf" || extension == "xml" || extension == "zip")
		return ("Content-Type: application/" + extension + "\r\n");
	return ("Content-Type: text/plain\r\n");
}

int	Serv::_handleGet()
{
	std::ifstream	file(this->_path.c_str() + 1);
	if (file.fail())
	{
		std::cerr << RED << "Error opening " << this->_path << "!\n" << RESET << std::endl;
		std::string response_body = "404 Not Found";
		std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: " + std::to_string(response_body.length()) + "\r\n\r\n" + response_body;
		send(this->_newSocket, response.c_str(), response.length(), 0);
		close(this->_newSocket);
		return (1);
	}

	file.seekg(0, std::ios::end);
	long	file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string	file_contents;
	file_contents.resize(file_size + 1);
	if (file.read(&file_contents[0], file_size).fail())
	{
		std::cerr << RED << "Error reading " << this->_path << "!\n" << RESET << std::endl;
		file.close();
		close(this->_newSocket);
		return (1);
	}
	file.close();

	std::string	content_type = get_content_type(this->_path);
	std::string	http_response = "HTTP/1.1 200 OK\r\n" + get_content_type(this->_path) + "Content-Length: " + std::to_string(file_size) + "\r\n\r\n" + file_contents;
	std::cout << GREEN << get_content_type(this->_path) << RESET << std::endl;
	write(this->_newSocket, &http_response[0], http_response.size());
	close(this->_newSocket);
	return (0);
}

void	Serv::_handleCgi(std::string method, int contentLength)
{
	int		cgi_input[2], cgi_output[2], status;
	pid_t	pid;
	char	c;

    if (pipe(cgi_input) < 0 || pipe(cgi_output) < 0)
		this->_perrorExit("pipe failed");
    if ((pid = fork()) < 0)
		this->_perrorExit("fork failed");

    if (pid == 0)	// child process
	{
        close(cgi_input[1]);
        close(cgi_output[0]);

        dup2(cgi_input[0], STDIN_FILENO);
        dup2(cgi_output[1], STDOUT_FILENO);

		// setenv("REQUEST_METHOD", method, 1);
        // setenv("SCRIPT_NAME", path, 1);
        // setenv("QUERY_STRING", query_string, 1);
        // setenv("CONTENT_TYPE", content_type, 1);
        // setenv("CONTENT_LENGTH", "69", 1);

		char	*cmds[2] = {(char *)(this->_path.c_str() + 1), NULL};
		execve(cmds[0], cmds, NULL);
		std::cerr << RED << "Failed to execve CGI" << RESET << std::endl; 
        exit(EXIT_FAILURE);
    }
	else	// parent process
	{
        close(cgi_input[0]);
        close(cgi_output[1]);

        if (method == "POST")
		{
            int n = read(this->_newSocket, &c, 1);
            int i = 0;
            while (n > 0 && i < contentLength) {
                write(cgi_input[1], &c, 1);
                n = read(this->_newSocket, &c, 1);
                i++;
            }
        }

		std::string	buffer(WS_BUFFER_SIZE, '\0');
        int n = read(cgi_output[0], &buffer[0], WS_BUFFER_SIZE);
        while (n > 0)
		{
            write(this->_newSocket, &buffer[0], n);
            n = read(cgi_output[0], &buffer[0], WS_BUFFER_SIZE);
        }

        close(cgi_input[1]);
        close(cgi_output[0]);

        waitpid(pid, &status, 0);
		close(this->_newSocket);
    }
}

void	Serv::_serverLoop()
{
	long	valread;

	while(1)
	{
		std::cout << CYAN << "Port: " << WS_PORT << "\nWaiting for new connection..." << RESET << std::endl;
		this->_newSocket = accept(this->_serverFd, NULL, NULL);
		if (this->_newSocket < 0)
			this->_perrorExit("Accept Error");
		std::string	buffer(WS_BUFFER_SIZE, '\0');
		valread = read(this->_newSocket, &buffer[0], WS_BUFFER_SIZE);
		buffer.resize(valread);

		std::string	method, query, contentType;
		int		contentLength = 0;
		std::istringstream	request(buffer);
		
		request >> method >> this->_path;
		if (this->_path == "/favicon.ico") // Ignore favicon
		{
			std::cout << RED << "Go away favicon" << RESET << std::endl;
			close(this->_newSocket);
			continue;
		}
		std::cout << buffer;
		if (method == "POST")
		{
			HttpPostResponse	postResponse(this->_newSocket, contentLength, valread, buffer);
			postResponse.handlePost();
		}
		else if (method == "GET" && this->_path != "/" && this->_path.find(".php") == std::string::npos && this->_path.find(".py") == std::string::npos && this->_path.find(".cgi") == std::string::npos) // Will be determined by the config
		{
			if (this->_handleGet())
				continue ;
		}
		else if (this->_path.find('.') != std::string::npos)
			this->_handleCgi(method, contentLength);
		else
		{
			HttpDefaultResponse	defaultResponse(this->_newSocket);
			defaultResponse.handleDefault();
		}
	}
}

void	Serv::runServer()
{
	this->_configManager.parseConfigFile();
	this->_setupServer();
	this->_serverLoop();
}