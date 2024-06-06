#ifndef COOKIE_HPP
# define COOKIE_HPP

#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include <unordered_map>

class Cookie 
{
	public:

		Cookie() {};
		Cookie(const std::string& name) : _name(name)
		{
			// std::cout << "cookie constructor called" << std::endl;
		}
		~Cookie() {};
		void setExpiresStr(const std::string& expires);
		void setExpiresTime(time_t expiresTime);
		void setValue(const std::string& path);
		void setName(const std::string& path);
    	// void setDomain(const std::string& domain);
		// void setPath(const std::string& path);

		std::string getName(void) const;
		std::string getValue(void) const;
		std::string getExpiresStr(void) const;
		time_t		getExpiresTime(void) const;
		// std::string getDomain(void) const;
		// std::string getPath(void) const;
		Cookie		&operator=(const Cookie &ref);

		// long int	_longTime;

	private:

		std::string _name;
		std::string _value;
		std::string _expiresStr;
		time_t		_expiresTime;
		// std::string _domain;
		// std::string _path;
};

class CookieJar 
{
	public:

		CookieJar()
		{
			// std::cout << "Cookie Jar constructor called" << std::endl;
			this->clientCookieName = "";
			this->clientCookieHash = "";
		}
		std::unordered_map<std::string, Cookie>	getCookieDB() const;
		std::string sendCookie() const;
		std::string	generateHash();

		long int 	generateExpirationTime();
		std::string generateExpirationStr(time_t time);
		


		Cookie	&operator[](const std::string& name);
		bool	checkExpiration(time_t time) const;
		bool	checkClientCookie(std::string key, std::string value);
		void	addCookie(const Cookie& cookie);
		void	insertCookie(const std::string& name);
		const	std::vector<Cookie>& getCookies(void) const;
		std::string dataResponse(void) const;

		std::string clientCookieName;
		std::string clientCookieHash;

		private:
    		std::unordered_map<std::string, Cookie> _cookieDB;
};

#endif