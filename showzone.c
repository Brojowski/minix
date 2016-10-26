#include <stdio.h>

#include "showzone.h"
#include "main.h"

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
    if (numArgs <= 1)
    {
        printf("usage: showzone <zone_number>");
        return;
    }

    showHead();
    for (int i = 0; i < BLOCK_SIZE; i+=8)
    {
        printf("%x",i);
        // Print content here

        printf("\n");
    }
}
