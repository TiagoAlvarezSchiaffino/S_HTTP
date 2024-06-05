/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   multiple_workers.cpp                                                     */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/06/05 19:18:46 by Tiago                    /   (_____/     */
/*   Updated: 2024/06/05 19:25:30 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc < 3)
	{
        std::cout << "Usage: " << argv[0] << " <script> <num_forks> [arg1] [arg2] ..." << std::endl;
        return 1;
    }
    char* script = argv[1];
    int num_forks = std::atoi(argv[2]);
    char* args[argc-1];
    args[0] = script;
    for(int i = 3; i < argc; i++)
        args[i-2] = argv[i];
    args[argc-2] = nullptr;
    for(int i = 0; i < num_forks; i++)
	{
        pid_t pid = fork();
        if(pid == 0) { // child process
            std::cout << "Running script " << i+1 << "..." << std::endl;
            execv(script, args); // run the script with arguments
            std::cout << "Error: failed to run script!" << std::endl;
            exit(1);
        }
    }
    int status;
    while(wait(&status) > 0);
    return 0;
}