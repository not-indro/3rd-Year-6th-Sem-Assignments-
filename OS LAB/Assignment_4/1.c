#include <stdio.h> // Header file for standard input and output
#include <unistd.h> // Header file for standard symbolic constants and types
#include <sys/ipc.h>    // Header file for interprocess communication
#include <sys/shm.h>   // Header file for shared memory
#include <stdlib.h> // Header file for standard library
#include <string.h> // Header file for string operations
#include <sys/wait.h> // Header file for waiting for process to change state

#define PAGE_SIZE 3 * sizeof(int)
#define PROJ_ID 1

int main()
{
    key_t key; // Key for our shared memory
    int shmid; // Shared memory id
    int *shm; // Pointer to our shared memory
    pid_t pid;// Process id

    key = 453;  // Generating a key for our shared memory
    if (key == -1) // Error while we tried to generate a key
    {
        perror("Error while Generating the Key"); 
        return 1;
    }
      // Creating a new shared memory
    shmid = shmget(key, PAGE_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1)
    {
        perror("Error While We tried to create a new shared memory\n");
        return 1; // Error while we tried to create a new shared memory
    }
// Attaching our shared memory in our parent process
    shm = shmat(shmid, NULL, 0);
    if (shm == (int *)-1)
    {
        perror("Error While we tried to attach our shared memory in our parent process\n"); // Error while we tried to attach our shared memory in our parent process
        return 1;
    }
// Generating a random number
    int num = rand() % 10 + 1;
    shm[0] = num;
    shm[1] = -1;
    pid = fork();
    if (pid == -1)
    {
        perror("Error while we creating child process\n");
        return 1;
    }
    if (pid == 0) // Child Process
    {

        while (1) //    Generating a random number
        {
            while (shm[1] != -1)
            {
                sleep(1);
            }
            shm = shmat(shmid, NULL, 0); // Attaching our shared memory in our child process
            sleep(1);
            if (shm == (int *)-1) 
            {
                perror("shmat\n"); // Error while we tried to attach our shared memory in our child process
                return 1;
            }
            int fact = 1;
            for (int i = 1; i <= shm[0]; i++)
            {
                fact *= i;
            }
            shm[1] = 5;
            printf("Factorial of %d is : %d\n", shm[0], fact); // Printing the factorial of the number
        }

        shmdt(shm); // Detaching our shared memory from our child process
    }
    else // Parent Process
    {

        while (1) //    Generating a random number
        {
            sleep(1);
            int num = rand() % 10 + 1;
            shm[0] = num;
            shm[1] = -1;
        }
        wait(NULL); // Waiting for the child process to complete

        shmdt(shm);// Detaching our shared memory from our parent process

        shmctl(shmid, IPC_RMID, NULL);//    Deleting our shared memory
    }
    return 0; // End of the program
}
