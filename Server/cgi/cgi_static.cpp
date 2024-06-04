/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cpptest.cpp                                                              */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/04 08:30:26 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/04 08:39:47 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main () {
	std::cout << "HTTP/1.1 200 OK\n";
	std::cout << "Content-type:text/html\r\n\r\n";
	std::cout << "<!DOCTYPE html>\n";
	std::cout << "<html lang=\"en\">\n";
	std::cout << "<head>\n";
	std::cout << "	<meta charset=\"UTF-8\">\n";
	std::cout << "	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
	std::cout << "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	std::cout << "	<title>Document</title>\n";
	std::cout << "</head>\n";
	std::cout << "<body>\n";
	std::cout << "Hello World 2 \n";
	std::cout << "	<img src=\"2mb.jpg\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<img src=\"south.png\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<img src=\"south.png\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<img src=\"south.png\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<img src=\"south.png\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<img src=\"south.png\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<img src=\"south.png\" alt=\"Image\" height=\"500px\">\n";
	std::cout << "	<video src=\"test.mp4\" controls></video>\n";
	std::cout << "	<video src=\"rickroll.mp4\" controls></video>\n";
	std::cout << "	<video src=\"rickroll.mp4\" controls></video>\n";
	std::cout << "	<video src=\"rickroll.mp4\" controls></video>\n";
	std::cout << "	<video src=\"rickroll.mp4\" controls></video>\n";
	std::cout << "	<video src=\"rickroll.mp4\" controls></video>\n";
	std::cout << " <video width=\"500px\" height=\"500px\" controls=\"controls\"> <source src=\"test.mp4\" type=\"video/mp4\"></video>";
	std::cout << "	<script src=\"./javascript.js\"></script>\n";
	std::cout << "</body>\n";
	std::cout << "</html>\n";

   return (0);
}