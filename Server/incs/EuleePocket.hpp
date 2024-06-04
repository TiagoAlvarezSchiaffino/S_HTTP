/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleePocket.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 16:53:59 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 17:59:41 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEPOCKET_HPP
# define EULEEPOCKET_HPP

# include <map>
# include <vector>
# include <unistd.h>
# include <sys/select.h>
# include <netinet/in.h>
# include "EuleeWallet.hpp"

# define RESET		"\033[0m"
# define RED		"\033[1;31m"
# define WS_TIMEOUT	3

enum	Mode
{
	READ,
	WRITE
};

// server block
struct EuleePocket
{
	EuleePocket(void);
	EuleePocket(std::string port, std::string root, std::string index, std::string domain, std::string autoIndex, std::string uploadSize, std::string errorPath);
	~EuleePocket(void);

	// member variables
	std::map<std::string, std::string>	envp;
	std::vector<EuleeWallet>			location;
	std::string							port, root, index, domain;
	std::string							autoIndex, uploadSize, errorPath;

	std::vector<int>					serverFd;
	std::vector<sockaddr_in>			serverAddr;
	std::string							path, method, buffer;
	int									socket, contentLength, valread;

	// member functions
	void	perrorExit(std::string msg, int exitTrue = 1);
	long	ft_select(int fd, void *buff, size_t size, Mode mode);
};

#endif