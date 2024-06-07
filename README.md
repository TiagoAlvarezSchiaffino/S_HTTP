# S_HTTP
S_HTTP is a server project written in C++ 98, drawing inspiration from Nginx configuration and behaviors.

The Hypertext Transfer Protocol (HTTP) is an application protocol for distributed, collaborative, hypermedia information systems. HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access.

While the primary function is to serve content, full implementation of HTTP also includes ways of receiving content from clients.

My S_HTTP project handles common methods such as GET, POST, PUT, DELETE, and HEAD methods, cookie and session management, as well as supporting multiple Common Gateway Interface (CGI) scripts.


## Project Demo
![Project Demo]


## Features
- Compatible with Google Chrome web browser
    - Note: Any other web browser may produce unexpected results
- Comparable with NGINX which is HTTP 1.1 compliant
- HTTP response status codes
- Default error pages are set
- Able to serve a fully static website
- Able to upload files
- Able to listen to multiple ports
- Able to handle and execute multiple CGI
- Supports cookies and session management

**Routes Settings**
- Define a list of accepted HTTP methods (limit_except)
- Setup HTTP redirection
- Setup server name
- Setup server ports
- Setup default error pages
- Limit client body size (client_max_body_size)
- Define a directory or a file from where the file should be searched (root)
- Turn on or off directory listing (autoindex)
- Set the default file to answer if the request is a directory (index)
- Execute CGI based on certain file extensions (.cgi)