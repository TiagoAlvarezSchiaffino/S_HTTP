/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cgi_toupper.cpp                                                          */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/05 10:35:10 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/05 11:48:31 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <unistd.h>

# define WS_BUFFER_SIZE 100000000

int    main(void)
{
    char	*buffer = new char[WS_BUFFER_SIZE];
	std::cin.getline(buffer, WS_BUFFER_SIZE);

    for (size_t i = 0; buffer[i]; i++)
        buffer[i] = toupper(buffer[i]);
    std::cout << buffer;

    	delete[] buffer;
    return (0);
}