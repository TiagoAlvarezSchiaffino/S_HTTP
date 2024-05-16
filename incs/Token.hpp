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
/*   Updated: 2024/05/16 03:59:29 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

enum Type
{
    CONTEXT,
    KEY,
    VALUE,
    SEMICOLON,
    OPEN_BRACE,
    CLOSE_BRACE
};

class Token
{
	public:
		Token(std::string context, Type type, int lineNum);
		~Token();
		std::string	getToken();
		int			getLineNum();
		Type		getType();
	private:
		std::string	_token;
		int			_lineNum;
		Type		_type;
};

#endif