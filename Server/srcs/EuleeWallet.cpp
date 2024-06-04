/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   EuleeWallet.hpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 18:19:48 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 05:33:27 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "EuleeWallet.hpp"

EuleeWallet::EuleeWallet(void) : priority(), valuePath(), root(), index(), include(), cgiPass(), cgiIndex(), cgiParam() {}

EuleeWallet::EuleeWallet(RegularExpression priority, std::string valuePath, std::string root, std::string index, std::string include, std::string cgiPass, std::string cgiIndex, std::string cgiParam) : priority(), valuePath(), root(), index(), include(), cgiPass(), cgiIndex(), cgiParam()
{
	this->priority = priority;
	this->valuePath = valuePath;
	this->root = root;
	this->index = index;
	this->include = include;
	this->cgiPass = cgiPass;
	this->cgiIndex = cgiIndex;
	this->cgiParam = cgiParam;
}

EuleeWallet::~EuleeWallet(void) {}