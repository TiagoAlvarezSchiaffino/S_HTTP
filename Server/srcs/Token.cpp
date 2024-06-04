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
/*   Updated: 2024/06/04 05:50:08 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(std::string token, Type type, int lineNum): type(type), token(token), lineNum(lineNum) {}

Token::~Token(void) {}