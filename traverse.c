#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "showzone.h"
#include "main.h"
#include "command.h"
#include "traverse.h"



void traverse(){

	int FIRST_DATA_BYTE = (1024 * 5/*super.s_firstdatazone*/);
	lseek(fd, FIRST_DATA_BYTE, SEEK_SET);
	read(fd, &inode, 32);
	
	printf("\nmode: %o \nUID: %d \ni_size: %d \ntime: %d \nGID: %u \nNLINKS: %u \nZONE: %d\n", inode.i_mode, inode.i_uid, inode.i_size, inode.i_time, inode.i_gid, inode.i_nlinks, inode.i_zone[1]);
}
	
