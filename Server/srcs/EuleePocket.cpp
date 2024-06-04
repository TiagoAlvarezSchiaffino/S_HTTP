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
/*   Updated: 2024/06/04 15:55:41 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleePocket.hpp"

EuleePocket::EuleePocket(void) : envp(), vectorLocation(), location(), portIndex(-1), _server() {}

EuleePocket::EuleePocket(EuleeWallet server, std::vector<EuleeWallet> vectorLocation) : envp(), vectorLocation(vectorLocation), location(), portIndex(-1), _server(server) {}

EuleePocket::~EuleePocket(void) {}

EuleePocket::mapped_type	&EuleePocket::operator[](key_type key)
{
	return (this->_server[key]);
}

EuleePocket::iterator	EuleePocket::begin(void)
{
	return (this->_server.begin());
}

EuleePocket::iterator	EuleePocket::end(void)
{
	return (this->_server.end());
}