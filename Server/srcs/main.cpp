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
/*   Updated: 2024/06/04 19:08:08 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/server.hpp"

int	main(int ac, char **av)
{
	std::string configFile = DEFAULT_CONFIG_PATH;

	if (ac > 2)
	{
		std::cout << YELLOW "Usage: ./webserv <config_file>" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	else if (ac == 2)
		configFile = av[1];
	std::cout << GREEN "Reading config file : " CYAN << configFile << RESET << std::endl;
	try {
		WebServer	webserver(configFile);
		webserver.runServer();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
	// (void)ac;
	// (void)av;
	// EuleeHand hand;

	// hand.sendHttp(404, "./html/error.html");
}