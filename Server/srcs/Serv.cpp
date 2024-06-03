/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Serv.cpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:54:16 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 14:28:42 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* Class constructor that takes in configFilePath string */
Serv::Serv(std::string configFilePath)
{
	this->_database = EuleeHand(configFilePath, ConfigManager(configFilePath));
}

Serv::~Serv(void) {}

void	Serv::runServer(void)
{
	this->_database.parseConfigFile();
	this->_database.printTokens();
	std::cout << GREEN "Config File Parsing Done..." RESET << std::endl;
}