#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<time.h>

#define PROJ_ID 1 // This is the project id for ftok

struct att{
	int roll;
	time_t tm;
};

int main(int argc,char* argv[]){ // This is the main function
	
	if(argc <2){
		return 1;
	}
	char* str = argv[1]; // This is the string which is passed as argument
    char *endptr; // This is the pointer which will point to the end of the string
    int num = (int) strtol(argv[1],&endptr,10); // This is the number which is passed as argument
    if(*endptr !='\0'){ // This is the condition to check if the argument is a valid integer

         printf("The Corresponding argument is not a valid Integer\n");
         return 1;
        }
	
	key_t key; // This is the key which will be used to create the shared memory
	key = ftok("./Key_h/abc.txt",1); // This is the key which will be used to create the shared memory
//	key = ftok("/Key-Helper.txt",PROJ_ID);
	if(key == -1){
		perror("ftok: ");
		return 1;
	}
	
	int shmid; // This is the shared memory id
	shmid = shmget(key,sizeof(struct att),IPC_CREAT|0777); // This is the shared memory id
	if(shmid==-1){
		perror("shmget in Student: ");
		return 1;
		
	}
	// Now I am attaching my shared memory into process space
	struct att *shm_ptr;
	shm_ptr = (struct att*) shmat(shmid,NULL,0); // This is the pointer which will point to the shared memory
	
		if(shm_ptr == (struct att*)-1){ // This is the condition to check if the shared memory is attached or not
			perror("shmat");
			return 1;
		} 
		shm_ptr[num-1].roll=num; // This is the roll number which is stored in the shared memory
		shm_ptr[num-1].tm=time(NULL); // This is the time which is stored in the shared memory
		
		shmdt(shm_ptr); // This is the function to detach the shared memory from the process space
		
	return 0;
}