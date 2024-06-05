/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Token.hpp                                                                */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 01:55:33 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 18:52:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include "server.hpp"

enum Type
{
    CONTEXT,
    KEY,
    VALUE,
    SEMICOLON,
    OPEN_BRACE,
    CLOSE_BRACE
};

struct Token
{
	Token(std::string context, Type type, int lineNum);
	~Token(void);

	Type		type;
	std::string	token;
	int			lineNum;
};

#endif