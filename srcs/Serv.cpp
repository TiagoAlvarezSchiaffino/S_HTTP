/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.cpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:54:16 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 04:14:43 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* Class constructor that takes in configFilePath string */
Serv::Serv(std::string configFilePath): _configFilePath(configFilePath)
{
	this->_configManager = ConfigManager(configFilePath);
}

Serv::~Serv() {}

void	Serv::runServer()
{
	this->_configManager.parseConfigFile();
	std::cout << "Server running" << std::endl;
}