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
/*   Updated: 2024/06/04 06:57:00 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	main(int ac, char **av)
{
	std::string configFile = DEFAULT_CONFIG_PATH;

	if (ac > 2)
	{
		std::cout << "Usage: ./serv <configFile>" << std::endl;
		return (EXIT_FAILURE);
	}
	else if (ac == 2)
		configFile = av[1];
	std::cout << "Using config file: " << configFile << std::endl;
	try {
		Serv	Serv(configFile);
		serv.runServer();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}