#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

#define SEM_NO 4  // Number of semaphores
#define PROJ_ID 1 // Project ID

#define P1(s) semop(s, &Pop1, 1); // P1(s) is equivalent to semop(s, &Pop1, 1);
#define P2(s) semop(s, &Pop2, 1); // P2(s) is equivalent to semop(s, &Pop2, 1);
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
	// int arr[4];
	key_t mykey; // mykey is a variable of type key_t
	pid_t pid;	 // pid is a variable of type pid_t
	int semid;	 // semid is a variable of type int
	int status;	 // status is a variable of type int

	union semnum
	{
		int val;			   // val is a variable of type int
		struct semid_ds *buf;  // buf is a pointer to a structure of type semid_ds
		struct seminfo *__buf; // __buf is a pointer to a structure of type seminfo

	} setvalArg;

	setvalArg.val = 1;

	Pop1.sem_num = 0;		 // Pop1 is a structure of type sembuf
	Pop1.sem_op = -1;		 // sem_op is a variable of type short
	Pop1.sem_flg = SEM_UNDO; // sem_flg is a variable of type short

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

	mykey = ftok("./Key_h/abc.txt", 1); // ftok() is used to generate a key

	if (mykey == -1) // ftok() returns -1 on failure
	{
		perror("ftok() failed: "); // perror() is used to print the error message
		exit(1);
	}
	semid = semget(mykey, SEM_NO, IPC_CREAT | 0777); // semget() is used to create a semaphore

	if (semid == -1)
	{
		perror("semget() failed: "); // perror() is used to print the error message
		exit(1);
	}

	status = semctl(semid, 0, SETVAL, setvalArg); // semctl() is used to set the value of a semaphore

	if (status == -1)
	{
		perror("semctl() failed"); // perror() is used to print the error message
		exit(1);
	}

	while (1)
	{
		sleep(10); // sleep() is used to suspend the execution of the process for 10 seconds
		P1(semid);
		printf("Hi ! I am p1 \n");

		V2(semid);
	}

	return 0;
}