/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Serv.hpp                                                                 */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/15 23:48:14 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/03 14:28:18 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
# define SERV_HPP

# include "EuleeHand.hpp"

class Serv
{
	public:
		Serv(std::string configFilePath);
		~Serv(void);

		void	runServer(void);

	private:
		EuleeHand	_database;
};

#endif