#include <sys/wait.h> // wait
#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <unistd.h>
#include <string.h>

#define ReadEnd 0// 0 is the read end of the pipe
#define WriteEnd 1 // 1 is the write end of the pipe
int sort_done = 0; // flag to check if sorting is done

struct student  // structure to store student information
{
    int roll;   // roll number
    char name[100]; // name of the student
};

void report_and_exit(const char *msg) // function to report error in message and exit
{
    perror(msg); // report error message
    exit(-1); // exit with error
}

int main()
{
    int i, n, pipeFDs[2], pipeFDs2[2]; // pipe file descriptors
    char buf; // buffer to read from pipe
    printf("No. Of Students: "); // prompt user for number of students
    scanf("%d", &n); // read number of students
    struct student ss[n]; // array of students

    if (pipe(pipeFDs) < 0) // create pipe
    {
        report_and_exit("pipeFD"); // report error and exit
    }
    if (pipe(pipeFDs2) < 0)
    {
        report_and_exit("pipeFD2"); 
    }
    pid_t cpid = fork(); // create child process
    if (cpid < 0)
    {
        report_and_exit("fork");
    }
    if (0 == cpid)
    {
        close(pipeFDs[WriteEnd]); // close write end of pipe
        close(pipeFDs2[ReadEnd]);
        i = 0;
        struct student ss[n];
        struct student temp;
        read(pipeFDs[ReadEnd], &ss, sizeof(ss)); // read from pipe
        for (int j = n - 1; j >= 0; j--) // bubble sort
        {
            for (int k = 0; k < j; k++) // bubble sort
            {
                if (ss[k].roll > ss[k + 1].roll) 
                {
                    temp.roll = ss[k].roll;
                    strcpy(temp.name, ss[k].name);
                    ss[k].roll = ss[k + 1].roll;
                    strcpy(ss[k].name, ss[k + 1].name);
                    ss[k + 1].roll = temp.roll;
                    strcpy(ss[k + 1].name, temp.name);
                }
            }
        }

        write(pipeFDs2[WriteEnd], &ss, sizeof(ss)); // write to pipe
        close(pipeFDs[ReadEnd]);
        close(pipeFDs2[WriteEnd]);
        exit(0);
    }
    else
    {
        close(pipeFDs[ReadEnd]); // close read end of pipe
        close(pipeFDs2[WriteEnd]); // close write end of pipe
        for (i = 0; i < n; i++) // read student information
        {
            printf("Roll: ");
            scanf("%d", &ss[i].roll);
            printf("Name: ");
            scanf("%s", ss[i].name);
        }

        write(pipeFDs[WriteEnd], &ss, sizeof(ss)); // write to pipe
        i = 0;
        struct student temp[n]; // array of students
        int child_status;

        wait(NULL);

        read(pipeFDs2[ReadEnd], &temp, sizeof(temp)); // read from pipe

        for (i = 0; i < n; i++) // print sorted student information
        {
            printf("Roll: %d, Name: %s\n", temp[i].roll, temp[i].name); // print sorted student information
        }

        close(pipeFDs[WriteEnd]); // close write end of pipe
        close(pipeFDs2[ReadEnd]);
        exit(0); // exit
    }
}