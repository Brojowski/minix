#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "showzone.h"
#include "main.h"
#include "command.h"
#include "traverse.h"



void traverse(char* flag){

	int FIRST_DATA_BYTE = (1024 * 5/*super.s_firstdatazone*/);
	lseek(fd, FIRST_DATA_BYTE, SEEK_SET);
	read(fd, &inode, 32);
	
	if ((lseek(fd, SEEK_SET, SEEK_SET)) != 0) {
        printf("There was a problem setting pointer to beginning of imagefile\n");
        exit(1);
		
		if (strcmp("-l", flag) == 0) {
        printf(" ***********************************************************\n");
        printf(" ** FILE TYPE NOTATION **\n");
        printf(" ** **\n");
        printf(" ** drwx------ : DIRECTORY ONLY ACCESSIBLE BY OWNER **\n");
        printf(" ** -rw-r--r-- : FILE ONLY ACCESSIBLE BY GROUP PERMISSIONS **\n");
        printf(" ** drwxr-xr-x : DIRECTORY WITH OPEN ACCESS **\n");
        printf(" **-rwxr-xr-x : FILE ANY USER CAN READ OR EXECUTE **\n");
        printf(" ***********************************************************\n\n");
    }
    }
	
	printf("\nmode: %o \nUID: %d \ni_size: %d \ntime: %d \nGID: %u \nNLINKS: %u \nZONE: %d\n", inode.i_mode, inode.i_uid, inode.i_size, inode.i_time, inode.i_gid, inode.i_nlinks, inode.i_zone[1]);
}
	
