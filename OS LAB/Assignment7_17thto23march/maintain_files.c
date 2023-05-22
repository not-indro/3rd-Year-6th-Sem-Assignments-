/*Name - Indranil Bain, Enrolment Number - 2020csb039, Assignment 17th march, */

#include <stdio.h>  // printf, scanf, puts, NULL
#include <stdlib.h> // srand, rand
#include <string.h> // strlen

#define BLOCK_SIZE 1024 // 1KB
/* in this part the init file hasbeen used  and the size hasbeen taken*/
int init_File_dd(const char *fname, int bsize, int bno)
{                                    // bsize = block size, bno = block number
    int size = 1024 + bsize * bno;   // 1024 bytes for metadata
    FILE *file = fopen(fname, "wb"); // create file
    if (file == NULL)
    { // if file is not created
        return -1;
    }
    fseek(file, size - 1, SEEK_SET); // seek to end of file
    fputc('\0', file);
    fseek(file, 0, SEEK_SET);                     // seek to beginning of file
    int n = bno;                                  // number of blocks
    int s = bsize;                                // size of each block
    int ubn = (n / 8) / s + ((n / 8) % s != 0);   // number of blocks for user block
    int fbn = 1 + ubn + (n * 4) / BLOCK_SIZE;     // number of blocks for free block
    char *ub = (char *)calloc(ubn, sizeof(char)); // user block
    if (ub == NULL)
    {                 // if memory is not allocated
        fclose(file); // close file
        return -1;    // return -1
    }
    ub[0] = 0b11111111;               // first 8 blocks are reserved
    fwrite(&n, sizeof(int), 1, file); // write number of blocks
    fwrite(&s, sizeof(int), 1, file); // write size of each block
    fwrite(&ubn, sizeof(int), 1, file);
    fwrite(&fbn, sizeof(int), 1, file);
    fwrite(ub, sizeof(char), ubn, file);
    free(ub);
    fclose(file);
    return 0;
}

int get_freeblock(const char *fname)
{                                     // get free block
    FILE *file = fopen(fname, "r+b"); // open file
    if (file == NULL)
    {
        return -1;
    }
    fseek(file, 0, SEEK_SET); // seek to beginning of file
    int n, s, ubn, fbn;       // number of blocks, size of each block, number of blocks for user block, number of blocks for free block
    fread(&n, sizeof(int), 1, file);
    fread(&s, sizeof(int), 1, file);
    fread(&ubn, sizeof(int), 1, file);
    fread(&fbn, sizeof(int), 1, file);
    char *ub = (char *)calloc(ubn, sizeof(char));
    if (ub == NULL)
    {                 // if memory is not allocated
        fclose(file); // close file
        return -1;
    }
    fread(ub, sizeof(char), ubn, file); // read user block
    int i, j, blockno;                  // block number
    for (i = 0; i < ubn; i++)
    {
        if (ub[i] != 0xff)
        {
            for (j = 0; j < 8; j++)
            {
                if (!((ub[i] >> j) & 1))
                {
                    ub[i] |= 1 << j;
                    blockno = i * 8 + j;
                    fseek(file, 1024 + blockno * s, SEEK_SET);   // seek to block
                    char *buf = (char *)calloc(s, sizeof(char)); // buffer
                    if (buf == NULL)
                    { // if memory is not allocated
                        free(ub);
                        fclose(file); // close file
                        return -1;
                    }
                    memset(buf, 0xff, s);               // set buffer to 0xff
                    fwrite(buf, sizeof(char), s, file); // write buffer to block
                    free(buf);                          // free buffer
                    fbn--;
                    fseek(file, sizeof(int) * 2 + sizeof(char) * ubn, SEEK_SET);
                    fwrite(&fbn, sizeof(int), 1, file);
                    fwrite(ub, sizeof(char), ubn, file);
                    free(ub);
                    fclose(file);
                    return blockno; // return block number
                }
            }
        }
    }
    free(ub);     // free user block
    fclose(file); // close file
    return -1;    // return -1
}

int free_block(const char *fname, int bno) {} // free block
FILE *file = fopen(fname, "r+b");             // open file
if (file == NULL)
{              // if file is not opened
    return -1; // return -1
}
