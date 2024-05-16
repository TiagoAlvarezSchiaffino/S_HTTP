/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   ConfigManager.cpp                                                        */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 01:59:12 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 02:00:49 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigManager.hpp"

ConfigManager::ConfigManager(): _configFilePath() {}

ConfigManager::ConfigManager(std::string configFilePath): _configFilePath(configFilePath) {}

ConfigManager::~ConfigManager() {}

ConfigManager	&ConfigManager::operator=(const ConfigManager &srcs)
{
	this->_fileBuffer = srcs._fileBuffer;
	this->_configFilePath = srcs._configFilePath;
	return (*this);
}

int	ConfigManager::parseConfigFile()
{
	if (this->_configFilePath.empty())
		throw std::runtime_error("Failed to open config file");
	this->_file.open(this->_configFilePath.c_str(), std::ios::binary);
	if (this->_file.is_open() == false)
		throw std::runtime_error("Failed to open config file");
	return (1);
}