# ************************************************************************** #
#                                                                            #
#                                                                            #
#   test                                                                     #
#                                                                            #
#   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             #
#                                                             / \__          #
#                                                            (    @\___      #
#                                                             /         O    #
#   Created: 2024/05/16 04:47:51 by Tiago                    /   (_____/     #
#   Updated: 2024/05/16 05:31:13 by Tiago                  /_____/ U         #
#                                                                            #
# ************************************************************************** #

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
    int cgi_input[2], cgi_output[2];
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

        // dup2(cgi_input[0], STDIN_FILENO);
        // dup2(cgi_output[1], STDOUT_FILENO);

		std::string content_length_str = std::to_string(content_length);

		// printf("Hello World\n");
		char	*cmd = strdup(path.c_str());
		execve(cmd, &cmd, NULL);
		// free(cmd);
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
            while (n > 0 && i < content_length)
			{
                write(cgi_input[1], &c, 1);
                n = read(new_socket, &c, 1);
                i++;
            }
        }

		// std::string	buffer(BUFFER_SIZE, '\0');
        char buffer[BUFFER_SIZE];

        // int n = read(cgi_output[0], &buffer[0], BUFFER_SIZE);
        int n = read(cgi_output[0], buffer, BUFFER_SIZE);
        while (n > 0)
		{
            write(new_socket, buffer, n);
            n = read(cgi_output[0], buffer, BUFFER_SIZE);
        }

        close(cgi_input[1]);
        close(cgi_output[0]);

        waitpid(pid, &status, 0);
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

	const char	*port_str;
	port_str = std::to_string(port).c_str();
	if (getaddrinfo("localhost", port_str, &hints, &res) != 0)
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
		printf("%s", buffer.c_str());

		std::string	method, path, query_string, content_type;
		int		content_length = 0;

		std::istringstream iss(buffer);
		iss >> method >> path;

		printf("Method: |%s|\nPath: |%s|\n", method.c_str(), path.c_str());
		if (method == "POST")
		{
			size_t	content_length_pos = buffer.find("Content-Length: ");
			if (content_length_pos != std::string::npos)
			{
				content_length_pos += std::strlen("Content-Length: ");
				content_length = std::stoi(buffer.substr(content_length_pos));
			}
		}
		else if (method == "GET" && path != "/" && path.find(".php") == std::string::npos && path.find(".py") == std::string::npos && path.find(".cgi") == std::string::npos)
		{
			std::ifstream	file(path.c_str() + 1);

			if (file.fail())
			{
				printf("Error opening file!\n");
				close(new_socket);
				continue;
			}

			file.seekg(0, std::ios::end);
			int	file_size = file.tellg();
			file.seekg(0, std::ios::beg);

			// fseek(fp, 0L, SEEK_END);
			// int file_size = ftell(fp);
			// fseek(fp, 0L, SEEK_SET);

			// Allocate memory for the file contents
			// char* file_contents = (char *)malloc(file_size);
			// if (file_contents == NULL)
			// {
			// 	printf("Error allocating memory!\n");
			// 	fclose(fp);
			// 	close(new_socket);
			// 	continue;
			// }

			// Read the file contents into memory
			std::string	file_contents;
			if (file.read(&file_contents[0], file_size).fail())
			{
				printf("Error reading file!\n");
				file.close();
				close(new_socket);
				continue;
			}

			// if (fread(file_contents, file_size, 1, fp) != 1)
			// {
			// 	printf("Error reading file!\n");
			// 	free(file_contents);
			// 	fclose(fp);
			// 	close(new_socket);
			// 	continue;
			// }

			// Close the file
			// fclose(fp);
			file.close();

			// Send the HTTP response
			// char http_response[1024];
			std::string	http_response = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n" + std::to_string(file_size) + "\r\n\r\n" + file_contents;
			// sprintf(http_response, "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", file_size);
			write(new_socket, &http_response[0], http_response.size());

			// Send the file contents
			write(new_socket, &file_contents[0], file_size);

			// Free the memory
			// free(file_contents);
			close(new_socket);
		}

		size_t	query_string_pos = path.find('?'); // Not sure if we need to handle this
		if (query_string_pos != std::string::npos)
			query_string = path.substr(query_string_pos + 1);

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
	return (0);
}

int	main(void)
{
	server(PORT);
	system("leaks -q server");
}