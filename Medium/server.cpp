/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   server.cpp                                                               */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 06:19:02 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 06:44:13 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <sys/socket.h>
// #include <sys/types.h>
#include <netinet/in.h>
// #include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "server.hpp"

void	perror_and_exit(std::string msg)
{
	perror(msg.c_str());
	exit(EXIT_FAILURE);
}

void	handle_cgi(int new_socket, std::string method, std::string path, int content_length)
{
    int cgi_input[2];
    int cgi_output[2];
    pid_t pid;
    char c;
    int status;

    if (pipe(cgi_input) < 0 || pipe(cgi_output) < 0)
		perror_and_exit("pipe failed");
    if ((pid = fork()) < 0)
		perror_and_exit("fork failed");

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
        setenv("CONTENT_LENGTH", "69", 1);

		char	*cmd = strdup(path.c_str());
		execve(cmd, &cmd, NULL);
		free(cmd);
        exit(EXIT_FAILURE);
    }
	else	// parent process
	{
        close(cgi_input[0]);
        close(cgi_output[1]);

        if (method == "POST")
		{
            int n = read(new_socket, &c, 1);
            int i = 0;
            while (n > 0 && i < content_length) {
                write(cgi_input[1], &c, 1);
                n = read(new_socket, &c, 1);
                i++;
            }
        }

		std::string	buffer(BUFFER_SIZE, '\0');
        int n = read(cgi_output[0], &buffer[0], BUFFER_SIZE);
        while (n > 0)
		{
            write(new_socket, &buffer[0], n);
            n = read(cgi_output[0], &buffer[0], BUFFER_SIZE);
        }

        close(cgi_input[1]);
        close(cgi_output[0]);

        waitpid(pid, &status, 0);
		close(new_socket);
    }
}

int	server(int port)
{
	int					server_fd, new_socket, addrlen;;
	long				valread;
	struct sockaddr_in	address;
	struct addrinfo		hints, *res;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo("localhost", std::to_string(port).c_str(), &hints, &res) != 0)
		perror_and_exit("getaddrinfo failed");

	memcpy(&address, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	address.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		server(port + 1);
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
		perror_and_exit("listen failed");

	while (1)
	{
		printf("Port: %d\nWaiting for new connection...\n", port);
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			perror_and_exit("accept failed");
		std::string	buffer(BUFFER_SIZE, '\0');
		valread = read(new_socket, &buffer[0], 30000);
		buffer.resize(valread);

		std::string	method, path, query_string, content_type;
		int		content_length = 0;

		std::istringstream iss(buffer);
		iss >> method >> path;
		if (path == "/favicon.ico") // Ignore favicon
		{
			printf("Fuck you favicon\n");
			close(new_socket);
			continue;
		}
		printf("%s", buffer.c_str());


		printf("Method: |%s|\nPath: |%s|\n", method.c_str(), path.c_str());
		if (method == "POST")
		{
			size_t	content_length_pos = buffer.find("Content-Length: ");
			if (content_length_pos != std::string::npos)
			{
				content_length_pos += std::strlen("Content-Length: ");
				content_length = std::stoi(buffer.substr(content_length_pos));
			}

			std::string	message_body;
			message_body.resize(content_length);
			valread = read(new_socket, &message_body[0], content_length);

			std::string response_body = "This is the response to your POST request.";
			std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response_body.length()) + "\r\n\r\n" + response_body;
			printf("Response: %s\n", response.c_str());
			// exit(1);
			send(new_socket, response.c_str(), response.length(), 0);
		}
		else if (method == "GET" && path != "/" && path.find(".php") == std::string::npos && path.find(".py") == std::string::npos && path.find(".cgi") == std::string::npos) // Will be determined by the config
		{
			// Open file
			std::ifstream	file(path.c_str() + 1);
			printf("Opened file...\n");
			if (file.fail())
			{
				printf("Error opening file!\n");
				close(new_socket);
				continue;
			}

			// Get file size
			file.seekg(0, std::ios::end);
			long	file_size = file.tellg();
			file.seekg(0, std::ios::beg);
			printf("Got file size...\n");

			// Read file
			std::string	file_contents;
			file_contents.resize(file_size + 1);
			if (file.read(&file_contents[0], file_size).fail())
			{
				printf("Error reading file!\n");
				file.close();
				close(new_socket);
				continue;
			}
			printf("Read file...\n");
			file.close();

			// Send file
			std::string	http_response = "HTTP/1.1 200 OK\r\nContent-Type: video/mp4\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n" + file_contents;
			printf("Sending file...\n");
			// printf("Response: %s\n", http_response.c_str());
			printf("File content: %s\n", file_contents.c_str());
			write(new_socket, &http_response[0], http_response.size());
			close(new_socket);
		}
		else
		{
			size_t	extension_pos = path.find('.');
			if (extension_pos != std::string::npos)
			{
				printf("Extension found!\n");
				handle_cgi(new_socket, method, path.c_str() + 1, content_length);
			}
			else
			{
				printf("Extension not found...\n");
				std::string hello = "HTTP/1.1 200 OK\nContent-Length: 12\n\nHello World!";
				write(new_socket, hello.c_str(), hello.length());
				printf("Hello message sent\n\n");
				close(new_socket);
			}
		}

	}
	return (0);
}

int	main(void)
{
	server(PORT);
}