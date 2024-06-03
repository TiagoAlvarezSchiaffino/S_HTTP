/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.cpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:13:39 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 14:27:20 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	main(int ac, char **av)
{
	std::string config_file = DEFAULT_CONFIG_PATH;

	if (ac > 2)
	{
		std::cout << YELLOW "Usage: ./webserv <config_file>" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	else if (ac == 2)
		config_file = av[1];
	std::cout << GREEN "Reading config file : " CYAN << config_file << RESET << std::endl;
	try {
		Serv	serv(config_file);
		serv.runServer();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}