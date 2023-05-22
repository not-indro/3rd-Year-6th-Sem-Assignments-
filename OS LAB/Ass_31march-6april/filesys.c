#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 512

// struct to store info of a seq
typedef struct
{
    int used_blocks;
    int free_blocks;
    char block_bitmap[MAX_BLOCKS];
} sequence_info;

// Struct to store info of a file
typedef struct
{
    int total_blocks;
    int block_size;
    int used_sequences;
    sequence_info sequences[MAX_SEQS];
} file_info;

int find_free_block(sequence_info *seq)
{
    for (int i = 0; i < 100; i++)
    {
        if (seq->block_bitmap[i] == 0)
        {
            return i;
        }
    }
    return -1; // no free block found
}

// read the bno th block
bool readblock(const char *fname, int seq_no, int bno, char *buffer)
{
    FILE *fp = fopen(fname, "rb");
    if (fp == NULL)
    {
        return 0; // file cannot be opened
    }

    seek(fp, get_block_position(seq_no, bno), SEEK_SET);
    if (fread(buffer, BLOCK_SIZE, 1, fp) != 1)
    {
        fclose(fp);
        return 0; // Failed to read block
    }

    fclose(fp);
    return 1; // Success
}

bool writeblock(char *fname, int seq_no, int bno, char *buffer)
{
    FILE *fp = fopen(fname, "r+b");
    if (fp == NULL)
    {
        fp = fopen(fname, "wb");
        if (fp == NULL)
        {
            return 0;
        }
    }
    // finding index block
    // if seq no. -1 create new
}

int main()
{
    FILE *f;
    init_File_dd("a.txt", 4096, 128);
    return 0;
}