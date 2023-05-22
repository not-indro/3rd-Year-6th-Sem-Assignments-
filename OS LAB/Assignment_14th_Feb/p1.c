#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct student
{
    int roll;
    char name[100];
};

int compare(const void *s1, const void *s2)
{
    struct student *stu1 = (struct student *)s1;
    struct student *stu2 = (struct student *)s2;

    if (stu1->roll < stu2->roll)
        return -1;
    else if (stu1->roll > stu2->roll)
        return 1;
    else
        return 0;
}
// function for write a structure to a pipe
void writeStruct(int fd, struct student s)
{
    write(fd, &s, sizeof(struct student));
}

// function for read a structure from a pipe
struct student readStruct(int fd)
{
    struct student s;
    read(fd, &s, sizeof(struct student));
    return s;
}

int main()
{

    // Code For Storing Information in the Structure
    int n;
    printf("Enter the No. of Student: ");
    scanf("%d", &n);
    struct student ss[n];

    for (int i = 0; i < n; i++)
    {
        printf("Enter Roll Number of Student %d :", i + 1);
        scanf("%d", &(ss[i].roll));
        printf("Enter Name of Student %d :", i + 1);
        scanf("%s", ss[i].name);
        //		gets(ss[i].name);
    }

    // Code for Creating the Pipe

    int pipefds[2];
    pid_t pid;

    if (pipe(pipefds) == -1)
    {
        perror("Pipe: ");
        exit(EXIT_FAILURE);
    }

    // Code for Creating Child process and sending the data to the child process
    pid = fork();
    if (pid == -1)
    {
        perror("Fork: ");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        // Inside the child process
        struct student newStudent[n];
        if (read(pipefds[0], newStudent, sizeof(newStudent)) == -1)
        {
            perror("read in the child: ");
            exit(1);
        }
        // sort the arrray
        //		quicksort(newStudent,0,n-1);
        qsort(newStudent, n, sizeof(struct student), compare);
        if (write(pipefds[1], newStudent, sizeof(newStudent)) == -1)
        {
            perror("writing in the Child: ");
            exit(1);
        }
    }
    else
    {
        // Inside the parent process
        if (write(pipefds[1], ss, sizeof(ss)) == -1)
        {
            perror("writing in the parent: ");
            exit(1);
        }
        printf("Details Before Sorting The structure:\n");
        for (int i = 0; i < n; i++)
        {
            printf("Name: ");
            puts(ss[i].name);
            printf("\tRoll No. : %d\n", ss[i].roll);
        }
        struct student sortedStudent[n];
        if (read(pipefds[0], sortedStudent, sizeof(sortedStudent)) == -1)
        {
            perror("read in the child: ");
            exit(1);
        }
        printf("Details After Sorting The structure:\n");
        for (int i = 0; i < n; i++)
        {
            printf("Name: ");
            puts(sortedStudent[i].name);
            printf("\tRoll No. : %d\n", sortedStudent[i].roll);
        }
    }
}