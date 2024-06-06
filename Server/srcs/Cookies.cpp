#include "../incs/Cookies.hpp"


void Cookie::setName(const std::string& name)
{
	this->_name = name;
}

void Cookie::setValue(const std::string& value)
{
	this->_value = value;
}

void Cookie::setExpiresStr(const std::string& expires)
{
	this->_expiresStr = expires;
}

void Cookie::setExpiresTime(time_t expiresTime)
{
	this->_expiresTime = expiresTime;
}


std::string Cookie::getName() const
{
	return (this->_name);
}

std::string Cookie::getValue() const
{
	return (this->_value);
}

std::string Cookie::getExpiresStr() const
{
	return (this->_expiresStr);
}

time_t		Cookie::getExpiresTime(void) const
{
	return (this->_expiresTime);
}

Cookie		&Cookie::operator=(const Cookie &ref)
{
	this->_name = ref._name;
	this->_value = ref._value;
	this->_expiresTime = ref._expiresTime;
	this->_expiresStr = ref._expiresStr;
	return (*this);
}

Cookie&		CookieJar::operator[](const std::string& name)
{
	return (this->_cookieDB[name]);
}

std::unordered_map<std::string, Cookie>	CookieJar::getCookieDB() const
{
	return (this->_cookieDB);
}

std::string	CookieJar::generateHash()
{
    std::string hash;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 35); // 36 characters in total (26 letters + 10 numbers)
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 32; i++)
        hash += characters[distrib(gen)];
	return (hash);
}



void CookieJar::insertCookie(const std::string& name)
{
    Cookie	newCookie(name);
	newCookie.setValue(this->generateHash());
	newCookie.setExpiresTime(this->generateExpirationTime());
	newCookie.setExpiresStr(this->generateExpirationStr(newCookie.getExpiresTime()));
    this->_cookieDB[name] = newCookie;
}

std::string CookieJar::generateExpirationStr(time_t time)
{
    std::ostringstream	outputStream;
	char				expiry_buf[32];

 	tm*	expiry_time = gmtime(&time);
    strftime(expiry_buf, sizeof(expiry_buf), "%a, %d-%b-%Y %H:%M:%S GMT", expiry_time);
    outputStream << expiry_buf;
    return (outputStream.str());
}

time_t CookieJar::generateExpirationTime()
{
	time_t curr_time;
	curr_time = time(NULL);

	tm *local_time = localtime(&curr_time);
	time_t local_time_t = mktime(local_time);
	tm *gmt_time = gmtime(&local_time_t);

	gmt_time->tm_sec += 60; // set expiration time here

 	time_t	expiry_time_t = mktime(gmt_time);
	return(expiry_time_t);
}

bool	CookieJar::checkExpiration(time_t cookieTime) const
{
	time_t curr_time;
	curr_time = time(NULL);

	tm *gmt_time = gmtime(&curr_time);
	// gmt_time->tm_sec += 24 * 60 * 50; // debug
	time_t	gmt_time_t = mktime(gmt_time);
	
	if (cookieTime >= gmt_time_t)
	{
		std::cout << "Not expired" << std::endl;
		return (0);
	}
	std::cout << "Expired" << std::endl;
	return (1);
}

std::string CookieJar::sendCookie() const
{
    std::string cookieStr;
    std::unordered_map<std::string, Cookie>::const_iterator it;

	cookieStr += "Set-Cookie: ";
    for (it = this->_cookieDB.begin(); it != this->_cookieDB.end(); ++it) 
    {
        cookieStr += it->second.getName() + "=" + it->second.getValue();
        if (!it->second.getExpiresStr().empty())
			cookieStr += "; expires=" + it->second.getExpiresStr();
        cookieStr += "; ";
    }
    return (cookieStr);
}

bool	CookieJar::checkClientCookie(std::string key, std::string value)
{
	std::unordered_map<std::string, Cookie>::iterator it;

	it = this->_cookieDB.find(key);
	if (it != this->_cookieDB.end())
	{
		if (value == it->second.getValue())
		{
			if (this->checkExpiration(it->second.getExpiresTime()))
				this->sendCookie();
			return (0);
		}
		return(1);
	}
	return (0);
}