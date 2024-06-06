/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   CookieJar.cpp                                                            */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/06 05:54:18 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/06 05:58:31 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "CookieJar.hpp"

CookieJar::CookieJar() {}

CookieJar::~CookieJar() {}

tm *CookieJar::_generateExpirationTime(int expireTimeSeconds)
{
    time_t curr_time;
    curr_time = time(NULL);

    time_t expiry_time_t = curr_time + expireTimeSeconds;
    tm *gmt_time = gmtime(&expiry_time_t);
	return (gmt_time);
}

std::string CookieJar::_generateExpirationStr(tm *expiry_time)
{
	char	expiry_buf[32];

	std::memset(expiry_buf, 0, sizeof(expiry_buf));
    strftime(expiry_buf, sizeof(expiry_buf), "%a, %d %b %Y %H:%M:%S GMT", expiry_time);
    return (std::string(expiry_buf));
}

std::string	CookieJar::_generateHash()
{
    std::string hash;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 35);
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 32; i++)
        hash += characters[distrib(gen)];
	return (hash);
}

Cookie	CookieJar::generateCookie(int socket)
{
	this->_jar[socket] = Cookie("cookieId", this->_generateHash(), this->_generateExpirationStr(this->_generateExpirationTime(WS_COOKIE_EXPIRE_TIME)));
	return (this->_jar[socket]);
}

Cookie	&CookieJar::operator[](int socket)
{
	return (this->_jar[socket]);
}