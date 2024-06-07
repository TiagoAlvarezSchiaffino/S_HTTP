/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeWallet.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 18:19:48 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/07 06:08:51 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeWallet.hpp"

EuleeWallet::EuleeWallet() : cgi(), _location() {}

EuleeWallet::EuleeWallet(value_type location) : cgi(), _location(location) {}

EuleeWallet::mapped_type	&EuleeWallet::operator[](const key_type &key)
{
	return (this->_location[key]);
}

EuleeWallet::~EuleeWallet() {}


EuleeWallet::iterator	EuleeWallet::begin()
{
	return (this->_location.begin());
}

EuleeWallet::iterator	EuleeWallet::end()
{
	return (this->_location.end());
}