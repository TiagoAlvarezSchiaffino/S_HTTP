/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleePocket.cpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 16:56:40 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/06 06:46:07 by Tiago                   /_____/ U        */
/*                                                                            */
/* ************************************************************************** */

#include "EuleePocket.hpp"

EuleePocket::EuleePocket() : vectorLocation(), location(), errorPage(), portIndex(-1), _server() {}

EuleePocket::EuleePocket(EuleeWallet server, std::vector<EuleeWallet> vectorLocation, std::map<int, std::string> errorPage) : vectorLocation(vectorLocation), location(), errorPage(errorPage), portIndex(-1), _server(server) {}

EuleePocket::~EuleePocket() {}

EuleePocket::mapped_type	&EuleePocket::operator[](key_type key)
{
	return (this->_server[key]);
}

EuleePocket::iterator	EuleePocket::begin()
{
	return (this->_server.begin());
}

EuleePocket::iterator	EuleePocket::end()
{
	return (this->_server.end());
}