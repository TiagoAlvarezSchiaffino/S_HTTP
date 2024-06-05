/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   server.hpp                                                               */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:30:39 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 18:53:31 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define RESET		"\033[0m"
# define BLACK		"\033[1;30m"
# define RED		"\033[1;31m"
# define GREEN		"\033[1;32m"
# define YELLOW		"\033[1;33m"
# define BLUE		"\033[1;34m"
# define MAGENTA	"\033[1;35m"
# define CYAN		"\033[1;36m"
# define WHITE		"\033[1;37m"

# include <map>
# include <vector>
# include <string>
# include <limits>
# include <netdb.h>
# include <sstream>
# include <fstream>
# include <cstring>
# include <iostream>
# include <algorithm>
# include <filesystem>
# include <fcntl.h>
# include <dirent.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>


#endif