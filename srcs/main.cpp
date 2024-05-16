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
/*   Updated: 2024/05/15 23:58:11 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	main(int ac, char **av)
{
	std::string config_file = DEFAULT_CONFIG_PATH;

	if (ac > 2)
	{
		std::cout << "Usage: ./server <config_file>" << std::endl;
		return (EXIT_FAILURE);
	}
	else if (ac == 2)
		config_file = av[1];
	std::cout << "Using config file: " << config_file << std::endl;
	Serv	serv(config_file);
	return (EXIT_SUCCESS);
}