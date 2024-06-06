/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   HttpCgiResponse.cpp                                                      */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/03 17:03:30 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/06 03:42:46 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpCgiResponse.hpp"

HttpCgiResponse::HttpCgiResponse(EuleeHand *database) : _database(database) {}

HttpCgiResponse::~HttpCgiResponse() {}

void    HttpCgiResponse::handleCgi()
{
    static int count = 0;
    std::ofstream   inFile(WS_TEMP_FILE_IN, std::ios::binary);
    std::string     toWrite = this->_database->buffer[this->_database->socket].substr(this->_database->buffer[this->_database->socket].find("\r\n\r\n") + std::strlen("\r\n\r\n"));
    std::cerr << "Writing size: " << toWrite.size() << std::endl;
    inFile.write(toWrite.c_str(), toWrite.size());
    inFile.close();
    std::string ext = this->_database->methodPath[this->_database->socket].substr(this->_database->methodPath[this->_database->socket].find_last_of("."));
    this->_database->addEnv("PATH_INFO=" + this->_database->cgi[ext]);
    std::cout << GREEN << "CGI Path: " << this->_database->cgi[ext].c_str() << RESET << std::endl;
    std::cerr << "Socket: " << this->_database->socket << std::endl;

    pid_t pid = fork();
    if (pid == 0)
    {
        int inFd = open(WS_TEMP_FILE_IN, O_RDONLY, 0777);
		int outFd = open(WS_TEMP_FILE_OUT, O_CREAT | O_TRUNC | O_RDWR, 0777);
        dup2(inFd, STDIN_FILENO);
        close(inFd);
        dup2(outFd, STDOUT_FILENO);
		close(outFd);
        char *args[3] = {(char *)this->_database->cgi[ext].c_str(), (char *)this->_database->methodPath[this->_database->socket].c_str(), NULL};
        std::cerr << count << ": Entering CGI -- ";
        execve(args[0], args, this->_database->envp);
        std::remove(WS_TEMP_FILE_IN);
        std::remove(WS_TEMP_FILE_OUT);
        std::cerr << RED << "Execve failed..." << RESET << std::endl;
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    std::cerr << " -- Exiting CGI: " << status << std::endl;
    std::string output = "";
    int     outfd2 = open(WS_TEMP_FILE_OUT, O_RDWR, 0777);
    char    *buffer = new char[WS_BUFFER_SIZE];
    long    bytes_read = 0, total = 0;
    while ((bytes_read = read(outfd2, buffer, WS_BUFFER_SIZE)) > 0)
    {
        output.append(buffer, bytes_read);
        total += bytes_read;
    }
    std::cerr << "Output size: " << total << " Actual: " << output.length() << std::endl;
    close(outfd2);
    size_t  startPos = output.find("\r\n\r\n") + std::strlen("\r\n\r\n");
    std::cout << "startPos: " << startPos << std::endl;
    std::string newOutput = output.substr(startPos);

    this->_database->response[this->_database->socket] = "HTTP/1.1 200 OK\r\n\r\n" + newOutput;
    std::cout << GREEN << "CGI ran successfully!" << RESET << std::endl;
    std::remove(WS_TEMP_FILE_IN);
    std::remove(WS_TEMP_FILE_OUT);
    std::cerr << "Socket: " << this->_database->socket << " done" << std::endl;
    count++;
}