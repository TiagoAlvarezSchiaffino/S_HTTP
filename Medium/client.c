/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Client.c                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 04:37:55 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 04:49:10 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "port.h"

int	main(void)
{
	int 				sock = 0;
	long				valread;
	char				*hello = "Hello from client";
	char				buffer[30000] = {0};
	struct sockaddr_in	serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket creation error\n");
		return (1);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		printf("Invalid address/ Address not supported\n");
		return (1);
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Connection failed\n");
		return (1);
	}

	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(sock, buffer, 30000);
	printf("%s\n", buffer);
	return (0);
}