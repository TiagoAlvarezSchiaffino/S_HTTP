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
/*   Updated: 2024/05/16 01:56:54 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

class Token
{
	public:
		enum Type
		{
			OPEN_BRACE,
			CLOSE_BRACE,
			SEMICOLON,
			TEXT,
			INVALID
		};
		Token(const std::string &context, Type type, int lineNum);
		~Token();
		std::string	getContext();
		int			getLineNum();
		Type		getType();
	private:
		std::string	_context;
		int			_lineNum;
		Type		_type;
};

#endif