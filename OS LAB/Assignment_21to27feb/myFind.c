/*Name - Indranil bain, Enrolment Number - 2020csb039*/
/* Assignment Date - 21st Feb*/
#include <stdio.h>    // for printf
#include <stdlib.h>   // for exit
#include <string.h>   // for strcmp
#include <dirent.h>   // for opendir, readdir, closedir
#include <sys/stat.h> // for stat, S_ISDIR, S_ISREG, S_ISLNK

void search_dir(char *path, char *match)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "Error: could not open directory '%s'\n", path); // stderr is the standard error stream
        exit(EXIT_FAILURE);
    }

    struct dirent *entry; // struct dirent is a structure that contains the following members:
    while ((entry = readdir(dir)) != NULL)
    { // readdir() returns a pointer to a dirent structure representing the next directory entry in the directory stream pointed to by dir. It returns NULL on reaching the end of the directory stream or if an error occurred.
        // Ignore "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Construct the full path for this entry
        char full_path[1024];                                                 // 1024 is the maximum length of a path on Linux
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name); // snprintf() is similar to sprintf(), but it will not write more than size bytes (including the terminating null byte ('\0')). If the output was truncated due to this limit then the return value is the number of characters (excluding the terminating null byte) which would have been written to the final string if enough space had been available. Thus, a return value of size or more means that the output was truncated. (See also below under NOTES.) The output is always null-terminated, unless size is zero.

        // Check if the entry is a directory
        struct stat statbuf; // struct stat is a structure that contains the following members:
        if (stat(full_path, &statbuf) == -1)
        {                                                                    // stat() returns information about a file. The file to be stat-ed is specified by path. The stat structure is defined in <sys/stat.h> and contains the following fields:
            fprintf(stderr, "Error: could not stat file '%s'\n", full_path); // stderr is the standard error stream
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(statbuf.st_mode))
        { // S_ISDIR() returns true if the file is a directory
            // Recurse into subdirectories
            search_dir(full_path, match);
        }
        else if (S_ISREG(statbuf.st_mode))
        {
            // Check if the entry matches the search string
            if (strstr(entry->d_name, match) != NULL)
            {
                printf("file %s matches %s\n", full_path, match); // stdout is the standard output stream
            }
        }
        else if (S_ISLNK(statbuf.st_mode))
        {
            // Check if the symlink matches the search string
            if (strstr(entry->d_name, match) != NULL)
            {
                printf("symlink %s matches %s\n", full_path, match);
            }
        }
        else
        {
            printf("Warning: unknown file type for '%s'\n", full_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{ // argc is the number of arguments passed to the program, including the name of the program. argv is an array of pointers to the arguments. The first argument, argv[0], is the name of the program. The last argument, argv[argc-1], is a null pointer.
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <path> <match>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *path = argv[1];  // argv[1] is the first argument passed to the program
    char *match = argv[2]; // argv[2] is the second argument passed to the program

    search_dir(path, match); // search_dir() is a function that searches a directory for a file or symlink that matches a string

    return 0; // return 0 means that the program exited successfully
}

