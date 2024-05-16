/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   server.c                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 05:56:22 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 06:47:04 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "port.h"

void handle_cgi(int new_socket, char* method, char* path, char* query_string, char* content_type, int content_length) {
    int cgi_input[2];
    int cgi_output[2];
    pid_t pid;
    char c;
    int status;

    if (pipe(cgi_input) < 0 || pipe(cgi_output) < 0) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // child process
        close(cgi_input[1]);
        close(cgi_output[0]);

        dup2(cgi_input[0], STDIN_FILENO);
        dup2(cgi_output[1], STDOUT_FILENO);

        char content_length_str[16];
        sprintf(content_length_str, "%d", content_length);

        setenv("REQUEST_METHOD", method, 1);
        setenv("SCRIPT_NAME", path, 1);
        setenv("QUERY_STRING", query_string, 1);
        setenv("CONTENT_TYPE", content_type, 1);
        setenv("CONTENT_LENGTH", content_length_str, 1);

		// execve(path, &path, NULL);
        execl(path, path, NULL);
        exit(EXIT_FAILURE);
    } else {  // parent process
        close(cgi_input[0]);
        close(cgi_output[1]);

        if (strcmp(method, "POST") == 0) {
            int n = read(new_socket, &c, 1);
            int i = 0;
            while (n > 0 && i < content_length) {
                write(cgi_input[1], &c, 1);
                n = read(new_socket, &c, 1);
                i++;
            }
        }

        char buffer[BUFFER_SIZE];
        int n = read(cgi_output[0], buffer, BUFFER_SIZE);
        while (n > 0) {
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

	char	*port_str;
	port_str = ft_itoa(port);
	if (getaddrinfo("localhost", port_str, &hints, &res) != 0)
	{
		free(port_str);
		perror("getaddrinfo failed");
		exit(EXIT_FAILURE);
	}
	free(port_str);

	memcpy(&address, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	address.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		printf("Trying port %d\n", port + 1);
		server(port + 1);
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printf("Port: %d\nWaiting for new connection...\n", port);
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		{
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		char	buffer[BUFFER_SIZE] = {0};
		valread = read(new_socket, buffer, 30000);
		printf("%s\n", buffer);

		char	method[16];
		char	path[128];
		char	query_string[128];
		char	content_type[128];
		int		content_length = 0;

		sscanf(buffer, "%s %s", method, path);

		printf("Method: %s path %s\n", method, path);
		if (strcmp(method, "POST") == 0)
		{
			char	*content_length_str = strstr(buffer, "Content-Length: ");
			if (content_length_str != NULL)
				sscanf(content_length_str, "Content-Length: %d", &content_length);
		}
		else if (strcmp(method, "GET") == 0 && strcmp(path, "/test.mp4") == 0)
		{
			printf("GETTING IMAGE\n");
			// Open the image file
			FILE* fp = fopen("test.mp4", "rb");
			if (fp == NULL)
			{
				printf("Error opening image file!\n");
				close(new_socket);
				continue;
			}

			// Get the size of the file
			fseek(fp, 0L, SEEK_END);
			int file_size = ftell(fp);
			fseek(fp, 0L, SEEK_SET);

			// Allocate memory for the file contents
			char* file_contents = malloc(file_size);
			if (file_contents == NULL)
			{
				printf("Error allocating memory!\n");
				fclose(fp);
				close(new_socket);
				continue;
			}

			// Read the file contents into memory
			if (fread(file_contents, file_size, 1, fp) != 1)
			{
				printf("Error reading file!\n");
				free(file_contents);
				fclose(fp);
				close(new_socket);
				continue;
			}

			// Close the file
			fclose(fp);

			// Send the HTTP response
			char http_response[1024];
			sprintf(http_response, "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", file_size);
			write(new_socket, http_response, strlen(http_response));

			// Send the file contents
			write(new_socket, file_contents, file_size);

			// Free the memory
			free(file_contents);
			close(new_socket);
		}

		char	*query_string_start = strchr(path, '?');
		if (query_string_start != NULL)
		{
			*query_string_start = '\0';
			strcpy(query_string, query_string_start + 1);
		}

		char	*extension = strrchr(path, '.');
		if (extension != NULL)
		{
			printf("Extension found!\n");
			handle_cgi(new_socket, method, path + 1, query_string, content_type, content_length);
		}
		else
		{
			printf("Extension not found...\n");
			char* hello = "HTTP/1.1 200 OK\nContent-Length: 12\n\nHello World!";
			write(new_socket, hello, strlen(hello));
			printf("Hello message sent\n\n");
			close(new_socket);
		}
	}
	return (0);
}

int	main(void)
{
	server(PORT);
	// char	**cmds = ft_split("cgi_env.cgi", ' ');
	// char	*cmd = "cgi_env.cgi";
	// execve(cmds[0], cmds, NULL);
	// execve(cmd, &cmd, NULL);
	// printf("Failed!");
}