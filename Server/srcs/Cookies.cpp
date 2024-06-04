#include "../incs/Cookies.hpp"


void Cookie::setPath(const std::string& path)
{
	this->_path = path;
}

void Cookie::setDomain(const std::string& domain)
{
	this->_domain = domain;
}

void Cookie::setExpires(const std::string& expires)
{
	this->_expires = expires;
}

std::string Cookie::getName() const
{
	return (this->_name);
}

std::string Cookie::getValue() const
{
	return (this->_value);
}

std::string Cookie::getExpires() const
{
	return (this->_expires);
}

std::string Cookie::getDomain() const
{
	return (this->_domain);
}

std::string Cookie::getPath() const
{
	return (this->_path);
}

void CookieJar::addCookie(const Cookie& cookie)
{
	this->_cookies.push_back(cookie);
}

const	std::vector<Cookie>& CookieJar::getCookies(void) const
{
	return (this->_cookies);
}

std::string CookieJar::dataResponse() const
{
	std::string cookieStr;
	std::vector<Cookie>::iterator it;

	for (size_t i = 0 ; i < this->_cookies.size() ; i++) 
	{
		cookieStr += this->_cookies[i].getName() + "=" + this->_cookies[i].getValue();
		if (!this->_cookies[i].getExpires().empty()) {
			cookieStr += "; expires=" + this->_cookies[i].getExpires();
		}
		if (!this->_cookies[i].getDomain().empty()) {
			cookieStr += "; domain=" + this->_cookies[i].getDomain();
		}
		if (!this->_cookies[i].getPath().empty()) {
			cookieStr += "; path=" + this->_cookies[i].getPath();
		}
		cookieStr += "; ";
	}
	return (cookieStr);
}

int		main(void)
{
	CookieJar jar;

	Cookie c1("my_name", "herp derp");

	c1.setDomain(".abcdef.com");
	c1.setExpires("Tue, 04-Jun-2024 08:54:03 GMT"); // current time
	c1.setPath("/");

	Cookie c2("your_name", "example");

	c2.setDomain(".yodada.com");
	c2.setExpires("Tue, 04-Jun-2024 08:54:03 GMT"); // current time
	c2.setPath("/");

	jar.addCookie(c1);
	jar.addCookie(c2);

	std::string result;

	result = jar.dataResponse();
	std::cout << jar.getCookies().size() << std::endl;
	std::cout << "result: " << result << std::endl;


	return (0);
}
