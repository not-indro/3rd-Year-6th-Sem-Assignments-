#include <stdio.h> // Header file for standard input and output
#include <unistd.h> // Header file for standard symbolic constants and types
#include <stdlib.h> //  Header file for standard library

#define MAX_ARGV 100 // for defining max size of argv
void forkAndExecute(char *filename, char *argv[])
{
    pid_t pid = fork(); // fork a child process
    if (pid == 0)
    {
        int status = execve(filename, argv, NULL); // execute the command

        if (status == -1)
            perror("execve failed"); // if execve fails, print error

        exit(0); // exit the child process
    }
}

int main(int argc, char *argv[]) // main function
{
    int idx = 1;       // index for argv
    while (idx < argc) // loop for executing commands
    {
        char *tempArgv[MAX_ARGV]; // temporary argv for storing arguments
        tempArgv[0] = argv[idx];  // first argument is the command
        int argvIdx = 1;
        idx++;

        while (idx < argc && argvIdx < MAX_ARGV && argv[idx][0] == '-') // loop for storing arguments
        {
            tempArgv[argvIdx] = argv[idx]; // storing arguments
            idx++;
            argvIdx++;
        }

        tempArgv[argvIdx] = NULL; // last argument is NULL

        forkAndExecute(tempArgv[0], tempArgv); //   fork and execute the command
    }
    return 0;
}