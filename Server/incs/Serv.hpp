/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.cpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:48:14 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 23:58:01 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
# define SERV_HPP

# include <string>
# include "ConfigManager.hpp"

class Serv
{
	public:
		Serv(std::string configFilePath);
		~Serv();
		void	runServer();

	private:
		std::string		_configFilePath;
		ConfigManager	_configManager;
};

#endif