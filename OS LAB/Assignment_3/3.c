#include<stdio.h> // Header file for standard input and output
#include<unistd.h> //   Header file for standard symbolic constants and types
#include<sys/types.h> // Header file for system types
#include<sys/wait.h>  
  // Header file for waiting for process to change state



int main(int argc,char*argv[]){         // main function

int n=argc-1; // number of processes
int i; // loop variable
for(i=0;i<n;i++){ // loop for creating processes
pid_t pid = fork(); // fork a child process


if(pid==0){ // child process
char* args[] = {argv[i+1],NULL}; // arguments for execve
execve(args[0],args,NULL); //   execute the command
}

}
int status; // status of child process
waitpid(pid,&status,0); // wait for child process to finish
if(WIFEXITED(status)) // check if child process exited normally
printf("Child Process executed normally\n"); // print message
else
printf("Child process exited abnormally\n");
}

while(n >0){ // loop for waiting for all processes to finish
wait(NULL); // wait for child process to finish
n--;
}


return 0; // exit the parent process

}