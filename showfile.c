#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "main.h"
#include "command.h"
#include "showzone.h"

unsigned short getInodeFromName(char *file)
{
    int block = super.s_firstdatazone;
    for (int i = 0; i < BLOCK_SIZE; i += 32)
    {
	    char buf[32];
        lseek(fd, (block * BLOCK_SIZE) + i, SEEK_SET);
	    int out = read(fd, buf, 32);
	    struct minix_dir_entry *dirent = (struct minix_dir_entry *) &buf;
        if (!strcmp(file, dirent->name))
        {
            return dirent->inode;
        }
    }
    return 0;
}

void printFunc(int zone)
{
    lseek(fd, zone * BLOCK_SIZE, SEEK_SET);
    for (int i = 0; i < BLOCK_SIZE; i+=16)
    {
        for (int x = 0; x < 16; x++)
        {
            char c;
            read(fd, &c, 1);
            printf("%x\t", c);
        }
        printf("\n");
    }
}

void showfile(int numArgs, char *args[])
{
    if (!isMounted)
    {
        printf("No image mounted.\n");
        return;
    }
    if (numArgs <= 1)
    {
        printf("usage: showfile <image-filename>\n");
        return;
    }

    printf("File: %s\n", args[1]);
    unsigned short node = getInodeFromName(args[1]);
    if (!node)
    {
        printf("No file found.\n");
        return;
    }
    struct minix_inode inode = getInode(node);
    for (int i = 0; i < 7; i++)
    {
        if (inode.i_zone[i])
        {
            printFunc(inode.i_zone[i]);
        }
    }
}