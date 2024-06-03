#include <iostream>
using namespace std;

int main () {
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
	cout << "Hello World 2 \n";
	cout << "	<img src=\"2mb.jpg\" alt=\"Image\" height=\"500px\">\n";
	// cout << "	<video src=\"test.mp4\" controls></video>\n";
	// cout << "	<video src=\"rickroll.mp4\" controls></video>\n";
	// cout << " <video width=\"500px\" height=\"500px\" controls=\"controls\"> <source src=\"test.mp4\" type=\"video/mp4\"></video>";
	// cout << "	<script src=\"./javascript.js\"></script>\n";
	cout << "</body>\n";
	cout << "</html>\n";
   
   return 0;
}