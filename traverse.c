#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "showzone.h"
#include "main.h"
#include "command.h"
#include "traverse.h"

void printDirent(struct minix_dir_entry *dirent)
{
		printf("\t%s\n", dirent->name);
}

void intToPermission(short bytes, char *output)
{
	/* Char should be of the form:
	 * 		0000 0000 0000 0???
	 *
	 */
	if (bytes > 7)
	{
	 output[0] = '?';
	 output[1] = '?';
	 output[2] = '?';
	 return;
	}
	
	short firstBitMask  = 0x0004;
	short firstBit = firstBitMask & bytes;	
	short secondBitMask = 0x0002;
	short secondBit = secondBitMask & bytes;
	short thirdBitMask  = 0x0001;
	short thirdBit = thirdBitMask & bytes;

	 output[0] = firstBit  ? 'r' : '-';
	 output[1] = secondBit ? 'w' : '-';
	 output[2] = thirdBit  ? 'x' : '-';
}

void printAdvDirent(struct minix_dir_entry *dirent)
{
	struct minix_inode node = getInode(dirent->inode);

	short MASK_file   = 0x8000;
	short MASK_first  = 0x01C0;
	short MASK_middle = 0x0038;
	short MASK_last   = 0x0007;
	char otherPerm[10];
	//char date[10];
	otherPerm[9] = 0;
	intToPermission((MASK_first & node.i_mode) >> 6, &otherPerm[0]);
	intToPermission((MASK_middle & node.i_mode) >> 3, &otherPerm[3]);
	intToPermission((MASK_last & node.i_mode), &otherPerm[6]);
	//strftime(date, 20, localtime(&(node.st_mtime)));


	printf("%c", (MASK_file & node.i_mode) ? '-' : 'd');
	printf("%s", (char *)&otherPerm);
	printf(" %d", node.i_uid);
	printf(" %d", node.i_size);
	printf(" %s", mtime(&node.st_mtime));
	printf(" %s\n",dirent->name);

}

/**
 * Return:
 *	True if fileName != ""
 */
bool printTraverse(int start, bool withList)
{
	lseek(fd,start,SEEK_SET);
	char buf[32];
	int out = read(fd, buf, 32);
	struct minix_dir_entry *dirent = (struct minix_dir_entry *) &buf;

	if (strcmp("",dirent->name))
	{
		if (withList)
		{
			printAdvDirent(dirent);
		}
		else
		{
			printDirent(dirent);
		}
	}

	if (DEBUG)
	{
		if (dirent->name == NULL)
		{
			printf("NULL == dirent->:'%s'\n",dirent->name);
		}
		else
		{
			printf("NULL != dirent->:'%s'\n",dirent->name);
		}
	}
	return !strcmp("",dirent->name);
}

void traverse(int numberArgs, char *args[])
{
	if (!isMounted)
	{
		printf("No image mounted.\n");
		return;
	}

	bool withList = strcmp("-l",args[1]) == 0;

	// Start at hardcoded root.
	int start = BLOCK_SIZE * super.s_firstdatazone;
	bool end = false;
	for (int i=0; (i<BLOCK_SIZE) && !end; i+=32)
	{
		end = printTraverse(start + i, withList);
	}
}
	
