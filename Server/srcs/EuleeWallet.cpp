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
/*   Updated: 2024/06/04 10:35:51 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeWallet.hpp"

EuleeWallet::EuleeWallet(void) : _location() {}

EuleeWallet::EuleeWallet(value_type location) : _location(location) {}

EuleeWallet::mapped_type	&EuleeWallet::operator[](const key_type &key)
{
	return (this->_location[key]);
}

EuleeWallet::~EuleeWallet(void) {}


EuleeWallet::iterator	EuleeWallet::begin(void)
{
	return (this->_location.begin());
}

EuleeWallet::iterator	EuleeWallet::end(void)
{
	return (this->_location.end());
}