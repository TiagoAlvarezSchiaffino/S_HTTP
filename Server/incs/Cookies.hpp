#ifndef COOKIE_HPP
# define COOKIE_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Cookie 
{
	public:
		Cookie(const std::string& name, const std::string& value) : _name(name), _value(value)
		{
			std::cout << "cookie constructor called" << std::endl;
		}

		void setExpires(const std::string& expires);
    	void setDomain(const std::string& domain);
		void setPath(const std::string& path);

		std::string getName(void) const;
		std::string getValue(void) const;
		std::string getExpires(void) const;
		std::string getDomain(void) const;
		std::string getPath(void) const;

	private:

		std::string _name;
		std::string _value;
		std::string _expires;
		std::string _domain;
		std::string _path;
};

class CookieJar 
{
	public:
		void	addCookie(const Cookie& cookie);
		const	std::vector<Cookie>& getCookies(void) const;
		std::string dataResponse(void) const;

		private:
    		std::vector<Cookie> _cookies;
};

#endif
