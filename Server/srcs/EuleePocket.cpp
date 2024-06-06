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
/*   Updated: 2024/06/06 05:58:47 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleePocket.hpp"

EuleePocket::EuleePocket() : vectorLocation(), location(), portIndex(-1), _server() {}

EuleePocket::EuleePocket(EuleeWallet server, std::vector<EuleeWallet> vectorLocation) : vectorLocation(vectorLocation), location(), portIndex(-1), _server(server) {}

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