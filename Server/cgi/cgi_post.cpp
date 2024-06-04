/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cgi_post.cpp                                                             */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 06:11:17 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 06:32:04 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

using namespace std;

int main()
{
	cout << "HTTP/1.1 200 OK\n";
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!DOCTYPE html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<head>\n";
	cout << "	<meta charset=\"UTF-8\">\n";
	cout << "	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
	cout << "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	cout << "	<title>Document</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "Testing CGI POST \n";
    cout << "   <form method=\"post\" enctype=\"multipart/form-data\">\n";
    cout << "   <label for=\"outfile\">Select a file:</label>\n";
    cout << "   <input type=\"file\" name=\"outfile\" id=\"outfile\">\n";
    cout << "   <input type=\"submit\" value=\"Submit\">\n";
    cout << "   </form>\n";
	cout << "</body>\n";
	cout << "</html>\n";

    return 0;
}