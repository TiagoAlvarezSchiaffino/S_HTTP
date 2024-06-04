/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeHand.cpp                                                            */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 14:20:49 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 18:08:02 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeHand.hpp"

EuleeHand::EuleeHand(void) : _configFilePath(), _configManager() {}

EuleeHand::EuleeHand(std::string configFilePath, ConfigManager const &configManager) : _configFilePath(configFilePath), _configManager(configManager) {}

EuleeHand::EuleeHand(const EuleeHand &ref) : _configFilePath(ref._configFilePath), _configManager(ref._configManager) {}

EuleeHand::~EuleeHand(void) {}

EuleeHand	&EuleeHand::operator=(const EuleeHand &ref)
{
	this->_configFilePath = ref._configFilePath;
	this->_configManager = ref._configManager;
	return (*this);
}

void	EuleeHand::printTokens(void)
{
	this->_configManager.printTokens();
}

void	EuleeHand::parseConfigFile(void)
{
	this->_configManager.parseConfigFile();
}

void	EuleeHand::configLibrary(void)
{
	this->_configManager.configLibrary();
}

void	EuleeHand::errorHandleShit(void)
{
	this->_configManager.errorHandleShit();
}

void	EuleeHand::parseConfigServer(void)
{
}