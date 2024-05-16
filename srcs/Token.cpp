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
/*   Updated: 2024/05/16 02:02:57 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(const std::string &context, Type type, int lineNum) : _context(context), _lineNum(lineNum), _type(type) {}

Token::~Token() {}

std::string	Token::getContext()
{
	return (this->_context);
}

int	Token::getLineNum()
{
	return (this->_lineNum);
}

Token::Type	Token::getType()
{
	return (this->_type);
}