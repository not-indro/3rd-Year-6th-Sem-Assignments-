#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> // Header file for waiting for process to change state
#include <sys/shm.h> // Header file for shared memory

int main(int argc, char const *argv[]) //main function
{
    int n, m, p, i, j, k; //n=rows of A, m=col of A, p=col of B
    printf("\nEnter the no. of rows of  A: "); 
    scanf("%d", &n);
    printf("\nEnter no. of col. of A: "); 
    scanf("%d", &m);
    printf("\nEnter No. of col. of B: ");
    scanf("%d", &p);
    int A[n][m]; 
    int B[m][p];
    printf("\nEnter elements of matrix A: "); 
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }
    printf("\nEnter elements of matrix B: "); 
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < p; j++)
        {

            scanf("%d", &B[i][j]);
        }
    }
    int shmid = shmget(IPC_PRIVATE, sizeof(int) * n * p, IPC_CREAT | 0666); //creating shared memory
    int(*C)[p] = shmat(shmid, NULL, 0); //attaching shared memory to process space
    for (i = 0; i < n; i++)
    {
        pid_t pid = fork(); //creating child process
        if (pid == 0)
        {
            for (j = 0; j < p; j++) //calculating product of matrices A and B
            {
                C[i][j] = 0;
                for (k = 0; k < m; k++)
                {
                    C[i][j] += A[i][k] * B[k][j]; //multiplying matrices
                }
            }
            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }

    printf("Product of matrices A and B: \n"); //printing product of matrices
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < p; j++)
        {
            printf("\nC[%d][%d]: %d", i, j, C[i][j]); //printing product of matrices
        }
    }

    shmdt(C); //detaching shared memory
    shmctl(shmid, IPC_RMID, NULL); //removing shared memory
    return 0; //returning 0
}
