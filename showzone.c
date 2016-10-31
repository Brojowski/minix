#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "showzone.h"
#include "main.h"
#include "command.h"

void showHead()
{
    printf("\t");
    for (int i = 0; i < 16; i++)
    {
        printf(" %x",i);
    }
    printf("\n");
}

void showzone(int numArgs, char *args[])
{
    if (!isMounted)
    {
        printf("No image mounted.\n");
        return;
    }
    if (numArgs <= 1)
    {
        printf("usage: showzone <zone_number>\n");
        return;
    }
    int block_num = atoi(args[1]);
    printf("Zone: %d\n", block_num);    
    off_t result = lseek(fd, block_num * BLOCK_SIZE, SEEK_SET);

    showHead();

    for (int i = 0; i < BLOCK_SIZE; i+=8)
    {
        printf("%x",i);
        // Print content here
        for (int x = 0; x < 16; x++)
        {
            char *c;
            read(fd, c, 1);
            printf("%c",*c);
        }

        printf("\n");
    }
}
