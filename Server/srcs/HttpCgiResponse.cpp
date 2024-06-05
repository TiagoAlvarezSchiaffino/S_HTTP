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
/*   Updated: 2024/06/04 14:30:29 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpCgiResponse.hpp"

HttpCgiResponse::HttpCgiResponse(EuleeHand database) : _database(database) {}

HttpCgiResponse::~HttpCgiResponse() {}

void    HttpCgiResponse::handleCgi()
{
    std::ofstream   inFile(WS_TEMP_FILE_IN, std::ios::binary);
    inFile << this->_database.bufferTemp.substr(this->_database.bufferTemp.find("\r\n\r\n") + std::strlen("\r\n\r\n"));
    inFile.close();

    pid_t pid = fork();
    if (pid == 0)
    {
        int inFd = open(WS_TEMP_FILE_IN, O_RDONLY, 0777);
		int outFd = open(WS_TEMP_FILE_OUT, O_CREAT | O_TRUNC | O_RDWR, 0777);
        dup2(inFd, STDIN_FILENO);
        close(inFd);
        dup2(outFd, STDOUT_FILENO);
		close(outFd);
        std::string ext = this->_database.methodPath.substr(this->_database.methodPath.find_last_of("."));
        std::cerr << GREEN << "CGI Path: " << this->_database.cgi[ext].c_str() << RESET << std::endl;
        char *args[3] = {(char *)this->_database.cgi[ext].c_str(), (char *)this->_database.methodPath.c_str(), NULL};
        this->_database.addEnv("PATH_INFO=" + this->_database.cgi[ext]);
        execve(args[0], args, this->_database.envp);
        std::remove(WS_TEMP_FILE_IN);
        std::remove(WS_TEMP_FILE_OUT);
        std::cerr << RED << "Execve failed..." << RESET << std::endl;
        exit(1);
    }
    waitpid(pid, NULL, 0);
    std::string output = "";
    int	outfd2 = open(WS_TEMP_FILE_OUT, O_RDWR, 0777);
    char    *buffer = new char[WS_TESTER_SIZE];
    std::memset(buffer, 0, WS_TESTER_SIZE);
    long bytes_read = 0, total = 0;
    while ((bytes_read = read(outfd2, buffer, WS_TESTER_SIZE)) > 0)
    {
        output.append(buffer, bytes_read);
        total += bytes_read;
        delete[] buffer;
        buffer = new char[WS_TESTER_SIZE];
        std::memset(buffer, 0, WS_TESTER_SIZE);
    }
    close(outfd2);
    size_t  startPos = output.find("\r\n\r\n") + std::strlen("\r\n\r\n");
    std::string newOutput = output.substr(startPos);

    std::string response = "HTTP/1.1 200 OK\r\n\r\n" + newOutput;
    
    // size_t  totalSent = 0;
    // long    val = this->_database.ft_select(this->_database.socket, &response[0], response.size() - totalSent, WRITE);
    // while (val > 0)
    // {
    //     totalSent += val;
	// 	std::cout << GREEN << "Sent: " << val << "\t\tTotal: " << totalSent << RESET << "\r";
    //     val = this->_database.ft_select(this->_database.socket, &response[totalSent], response.size() - totalSent, WRITE);
    //     if (val < 0)
    //     {
    //         std::cout << RED << "VAL ERROR" << std::endl;
    //         break ;
    //     }
    // }
    
    std::cout << GREEN << "CGI ran successfully!" << std::endl;
    std::remove(WS_TEMP_FILE_IN);
    std::remove(WS_TEMP_FILE_OUT);
    delete[] buffer;
    close(this->_database.socket);
}