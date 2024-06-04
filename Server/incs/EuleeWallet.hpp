/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeWallet.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 17:59:56 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 05:32:44 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EULEEWALLET_HPP
# define EULEEWALLET_HPP

# include <string>
# include <iostream>

enum	RegularExpression
{
	NOT_SPECIFIED,		// everything else
	CASE_SENSITIVE,		// ~
	CASE_INSENSITIVE,	// ~*
	IGNORE_OTHERS,		// ^~
	EXACT_MATCH			// =
};

// location block
struct EuleeWallet
{
	EuleeWallet(void);
    EuleeWallet(RegularExpression priority, std::string valuePath, std::string root, std::string index, std::string include, std::string cgiPass, std::string cgiIndex, std::string cgiParam);
	~EuleeWallet(void);

	RegularExpression	priority;
	std::string			valuePath, root, index, include;
	std::string			cgiPass, cgiIndex, cgiParam;
};

#endif