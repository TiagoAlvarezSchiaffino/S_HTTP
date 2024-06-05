#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

// Set a cookie and send it to the client
void set_cookie(string& response, const string& key, const string& value, const string& domain, const string& path, time_t max_age, bool secure) {
    ostringstream cookie;
    cookie << key << "=" << value;
    if (!domain.empty()) {
        cookie << "; Domain=" << domain;
    }
    if (!path.empty()) {
        cookie << "; Path=" << path;
    }
    if (max_age > 0) {
        cookie << "; Max-Age=" << max_age;
    }
    if (secure) {
        cookie << "; Secure";
    }
    response += "Set-Cookie: " + cookie.str() + "\r\n";
}

// Handle an HTTP request and send an HTTP response with a cookie
string handle_request(const string& request) {
    // Parse the HTTP request to retrieve any cookies
    string cookie_value;
    size_t cookie_pos = request.find("Cookie: ");
    if (cookie_pos != string::npos) {
        cookie_pos += 8;
        size_t cookie_end_pos = request.find("\r\n", cookie_pos);
        if (cookie_end_pos != string::npos) {
            cookie_value = request.substr(cookie_pos, cookie_end_pos - cookie_pos);
        }
    }

    // Set a cookie with the user's ID
    string user_id = "123456789";
    time_t max_age = 60 * 60 * 24; // 1 day
    string response = "HTTP/1.1 200 OK\r\n";
    set_cookie(response, "user_id", user_id, "", "/", max_age, true);

    // Send the HTTP response to the client
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 13\r\n";
    response += "\r\n";
    response += "Hello, world!";
    return response;
}

int main() {
    // Example usage
    string request = "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n";
    string response = handle_request(request);
    cout << response << endl;
    return 0;
}