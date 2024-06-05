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
/*   Updated: 2024/06/04 18:17:44 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeWallet.hpp"

EuleeWallet::EuleeWallet(void) : cgi(), _location() {}

EuleeWallet::EuleeWallet(value_type location) : cgi(), _location(location) {}
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