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
/*   Updated: 2024/06/03 14:26:06 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(std::string token, Type type, int lineNum): _type(type), _token(token), _lineNum(lineNum) {}

Token::~Token(void) {}

std::string	Token::getToken(void)
{
	return (this->_token);
}

int	Token::getLineNum(void)
{
	return (this->_lineNum);
}

Type	Token::getType(void)
{
	return (this->_type);
}