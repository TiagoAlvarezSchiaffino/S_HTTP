/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleePocket.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 16:53:59 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 16:56:31 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEPOCKET_HPP
# define EULEEPOCKET_HPP

# include <string>
# include <vector>
# include <iostream>

struct EuleePocket
{
	EuleePocket(void) {};
	~EuleePocket(void) {};

	// server block
	std::string					root;
	std::vector<std::string>	index;

	std::vector<std::string>	server_name;
	std::vector<std::string>	location_path;
};

#endif