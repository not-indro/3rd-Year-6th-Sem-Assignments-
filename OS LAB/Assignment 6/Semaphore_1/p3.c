#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

#define SEM_NO 4  // Number of semaphores
#define PROJ_ID 1 // Project ID

#define P1(s) semop(s, &Pop1, 1); // P1(s) is equivalent to semop(s, &Pop1, 1);
#define P2(s) semop(s, &Pop2, 1);
#define P3(s) semop(s, &Pop3, 1);
#define P4(s) semop(s, &Pop4, 1);
#define V1(s) semop(s, &Vop1, 1); // V1(s) is equivalent to semop(s, &Vop1, 1);
#define V2(s) semop(s, &Vop2, 1);
#define V3(s) semop(s, &Vop3, 1);
#define V4(s) semop(s, &Vop4, 1);

struct sembuf Pop1; // Pop1 is a structure of type sembuf
struct sembuf Pop2;
struct sembuf Pop3;
struct sembuf Pop4;
struct sembuf Vop1; // Vop1 is a structure of type sembuf
struct sembuf Vop2;
struct sembuf Vop3;
struct sembuf Vop4;

int main()
{
    key_t mykey; // mykey is a variable of type key_t
    pid_t pid;   // pid is a variable of type pid_t
    int semid;
    int status;

    union semnum // semnum is a union of type int, struct semid_ds *buf, struct seminfo *__buf
    {
        int val;
        struct semid_ds *buf;  // buf is a pointer to a structure of type semid_ds
        struct seminfo *__buf; // __buf is a pointer to a structure of type seminfo

    } setvalArg; // setvalArg is a variable of type semnum

    setvalArg.val = 1; // setvalArg.val is equivalent to setvalArg.val = 1

    Pop1.sem_num = 0;        // Pop1.sem_num is equivalent to Pop1.sem_num = 0
    Pop1.sem_op = -1;        // Pop1.sem_op is equivalent to Pop1.sem_op = -1
    Pop1.sem_flg = SEM_UNDO; // Pop1.sem_flg is equivalent to Pop1.sem_flg = SEM_UNDO

    Pop2.sem_num = 1;
    Pop2.sem_op = -1;
    Pop2.sem_flg = SEM_UNDO;

    Pop3.sem_num = 2;
    Pop3.sem_op = -1;
    Pop3.sem_flg = SEM_UNDO;

    Pop4.sem_num = 3;
    Pop4.sem_op = -1;
    Pop4.sem_flg = SEM_UNDO;

    Vop1.sem_num = 0;
    Vop1.sem_op = 1;
    Vop1.sem_flg = SEM_UNDO;

    Vop2.sem_num = 1;
    Vop2.sem_op = 1;
    Vop2.sem_flg = SEM_UNDO;

    Vop3.sem_num = 2;
    Vop3.sem_op = 1;
    Vop3.sem_flg = SEM_UNDO;

    Vop4.sem_num = 3;
    Vop4.sem_op = 1;
    Vop4.sem_flg = SEM_UNDO;

    mykey = ftok("./Key_h/abc.txt", 1); // ftok() is used to generate a unique key

    if (mykey == -1) // ftok() returns -1 on failure
    {
        perror("ftok() failed: ");
        exit(1);
    }
    semid = semget(mykey, SEM_NO, IPC_CREAT | 0777); // semget() is used to create a semaphore set

    if (semid == -1)
    {
        perror("semget() failed: "); // semget() returns -1 on failure
        exit(1);
    }

    status = semctl(semid, 2, SETVAL, setvalArg); // semctl() is used to perform operations on a semaphore set

    if (status == -1) // semctl() returns -1 on failure
    {
        perror("semctl() failed"); // semctl() returns -1 on failure
        exit(1);
    }

    while (1) // Infinite loop
    {
        sleep(5);                  // sleep() is used to suspend the execution of the calling process for a specified time
        P4(semid);                 // P4() is equivalent to semop(semid, &Pop4, 1);
        printf("Hi ! I am P 3\n"); // printf() is used to print the string
        V3(semid);                 // V3() is equivalent to semop(semid, &Vop3, 1);
    }

    return 0; // return 0 to the calling function
}