/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cgi_post.cpp                                                             */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 04:26:19 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 04:31:37 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

int main() {
    std::string content_length_str = std::getenv("CONTENT_LENGTH");
    int content_length = std::stoi(content_length_str);

    // Read the message body from the client
    std::string message_body;
    message_body.resize(content_length);
    std::cin.read(&message_body[0], content_length);

    // Process the message body according to your application's requirements
    // For example, parse the message body to extract form data
    std::string name;
    std::string email;
    std::string message;

    size_t name_start_pos = message_body.find("name=") + std::strlen("name=");
    size_t name_end_pos = message_body.find("&", name_start_pos);
    name = message_body.substr(name_start_pos, name_end_pos - name_start_pos);

    size_t email_start_pos = message_body.find("email=") + std::strlen("email=");
    size_t email_end_pos = message_body.find("&", email_start_pos);
    email = message_body.substr(email_start_pos, email_end_pos - email_start_pos);

    size_t message_start_pos = message_body.find("message=") + std::strlen("message=");
    message = message_body.substr(message_start_pos);

    // Generate a response
    std::string response_body = "<html><body><h1>Form Submission Received</h1>"
                                "<p>Name: " + name + "</p>"
                                "<p>Email: " + email + "</p>"
                                "<p>Message: " + message + "</p>"
                                "</body></html>";
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response_body.length()) +
                           "\r\nContent-Type: text/html\r\n\r\n" + response_body;

    // Send the response to the client
    std::cout << response;

    return 0;
}