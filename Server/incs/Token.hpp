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
/*   Updated: 2024/06/03 14:16:10 by Tiago                  /_____/ U         */
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
		~Token(void);

		Type		getType(void);
		std::string	getToken(void);
		int			getLineNum(void);

	private:
		Type		_type;
		std::string	_token;
		int			_lineNum;
};

#endif