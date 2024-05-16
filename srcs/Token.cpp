/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Token.cpp                                                                */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 02:01:03 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 04:11:04 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(std::string token, Type type, int lineNum) : _token(token), _lineNum(lineNum), _type(type) {}

Token::~Token() {}

std::string	Token::getToken()
{
	return (this->_token);
}

int	Token::getLineNum()
{
	return (this->_lineNum);
}

Type	Token::getType()
{
	return (this->_type);
}