/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleePocket.cpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 16:56:40 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 10:32:15 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleePocket.hpp"

EuleePocket::EuleePocket(void) : location(), serverFd(), serverAddr(), path(), method(), buffer(), socket(), contentLength(), valread(), _envp(), _server() {}

EuleePocket::EuleePocket(EuleeWallet server, std::vector<EuleeWallet> location) : location(location), serverFd(), serverAddr(), path(), method(), buffer(), socket(), contentLength(), valread(), _envp(), _server(server) {}

EuleePocket::~EuleePocket(void) {}

EuleePocket::mapped_type	&EuleePocket::operator[](key_type key)
{
	return (this->_server[key]);
}

EuleePocket::iterator	EuleePocket::begin(void)
{
	return (this->_server.begin());
}

EuleePocket::iterator	EuleePocket::end(void)
{
	return (this->_server.end());
}

void	EuleePocket::perrorExit(std::string msg, int exitTrue)
{
	std::cerr << RED << msg << ": ";
	perror("");
	std::cerr << RESET;
	if (exitTrue)
		exit(EXIT_FAILURE);
}

long	EuleePocket::ft_select(int fd, void *buff, size_t size, Mode mode)
{
	fd_set	readFds, writeFds;
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);
	FD_SET(fd, (mode == READ) ? &readFds : &writeFds);

	timeval	timeout;
	timeout.tv_sec = WS_TIMEOUT;
	timeout.tv_usec = 0;

	int	ret = select(FD_SETSIZE, &readFds, &writeFds, NULL, &timeout);
	if (ret == -1)
	{
		this->perrorExit("Select Error", 0);
		return (-1);
	}
	else if (ret == 0)
	{
		std::cout << RED << "Select timeout!" << RESET << std::endl;
		return (0);
	}

	long	val = 0;
	if (FD_ISSET(fd, &readFds) && mode == READ)
	{
		val = recv(fd, buff, size, 0);
		if (val == -1)
			this->perrorExit("Read Error", 0);
	}
	else if (FD_ISSET(fd, &writeFds) && mode == WRITE)
	{
		val = send(fd, buff, size, 0);
		if (val == -1)
			this->perrorExit("Write Error", 0);
	}
	return (val);
}