#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "main.h"
#include "showzone.h"

#include "command.h"
#include "traverse.h"

/*
 * How to add additional commands:
 *  1. Include your file:
 *          #include "yourfile.h"
 *  2. In main() add your command with createCommand():
 *          createCommand("cmd_name",&cmd_method);
 */

struct command_node *last;

/**
 * Purpose:
 *  End user input loop.
 */
void quit()
{
    stop = true;
}

/**
 * Purpose:
 *  Print all of the commands.
 */
void help()
{
    printf("Commands:\n");
    struct command_node *cmd = commands;
    do
    {
        printf("\t%s\n", cmd->name);
    } while ((cmd = cmd->next));
}

/**
 * Purpose:
 *  Add a command for the minix terminal.
 * @params:
 *  char *name: the name of the command.
 *  commandFunction method: the method to execute when the command is run.
 */
void createCommand(char *name, commandFunction method)
{
    struct command_node *new_cmd = (struct command_node *) malloc(sizeof(struct command_node));
    new_cmd->name = name;
    new_cmd->run = method;
    new_cmd->cmdLen = strlen(name);

    if (!commands)
    {
        commands = new_cmd;
        last = commands;
    }
    else
    {
        last->next = new_cmd;
        last = last->next;
    }
}

// Use a standard way rather than write one because standards are good.
// http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char) *str))
    {
        str++;
    }

    if (*str == 0)
    {  // All spaces?
        return str;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
    {
        end--;
    }

    // Write new null terminator
    *(end + 1) = 0;

    return str;
}

/**
 * Purpose:
 *  1. Read user input.
 *  2. Parse command.
 *  3. Parse args.
 *  4. Find and run command.
 */
void prompt()
{
    char buf[MAX_INPUT_SIZE];
    // There seems to be a problem with data being
    // left in buf if 6+ chars were read. This messes
    // up the next command. Clearing the buffer seems
    // to fix this.
    for (int i = 0; i < MAX_INPUT_SIZE; i++)
    {
        buf[i] = 0;
    }
    
    write(STD_OUTPUT, PROMPT_SYMBOL, strlen(PROMPT_SYMBOL));
    int rawRead = read(STD_INPUT, buf, MAX_INPUT_SIZE);

    // Trim spaces that could cause errors.
    char *trimmed = trimwhitespace(buf);
    int left = strlen(trimmed);

    char *args[left];

    // If there are no spaces, the command
    // is the same as the trimmed command.
    char *cmdStr = trimmed;
    args[0] = trimmed;
    int wordsFound = 1;
    for (int i = 0; i < left; i++)
    {
        if (trimmed[i] == ' ')
        {
            trimmed[i] = 0;
            args[wordsFound] = &trimmed[i+1];
            wordsFound++;
        }
    }
    //sometimes this number is wrong according to debug.
    //not sure how yet but believe this is the source
    //of your flag problem. (at least one possible source)	
    int cmdLen = strlen(args[0]);

    if (DEBUG)
    {
        for (int i = 0; i < wordsFound; i++)
        {
            printf("\t----%d:%s\n",i,args[i]);
        }
    }

    bool found = false;
    for (struct command_node *cmd = commands; !found && cmd; cmd = cmd->next)
    {
        if (DEBUG)
        {	
            printf("\t----%d=%d\n", cmd->cmdLen, cmdLen);
            printf("\t----'%s'='%s'\n", cmd->name, cmdStr);
        }

        /* This will short circuit if the lengths
         * do not match. Ints are more efficient
         * to compare than strings so compare size
         * before doing strcmp.
         * If the strings are same strcmp returns 0.
         */
        if (cmd->cmdLen == cmdLen && !strcmp(cmd->name, cmdStr))
        {
            cmd->run(wordsFound, args);		
            found = true;                
		    break;
            if (DEBUG)
            {
                printf("\t----RUN COMMAND\n");
            }
	    }
    }

    if (!found)
    {
        printf("%s: command not found\n", cmdStr);
    }
}

// mount selected file, throw error if file is invalid.
// map superblock attributes to variables in 'command.h'

void minimount(int numArgs,char *args[]){
	
	char *imagefile;
	
	// Check for argument	
	if (numArgs > 1){
        	imagefile = args[1];
    	}
	
	else{
        	return;
	}
	
	// Check for valid file	
	fd = open(imagefile, O_RDONLY);
	
	if(fd == -1){
		printf("\nInvalid Input, file wasn't mounted\n");
		return;	
	}
	
	// Check to see if an img file is already mounted
	if(isMounted == 1){
		printf("\nA file is already mounted, unmount to mount a different one\n");
		return;		
	}
	
	lseek(fd, 1024, SEEK_SET);
	read(fd, &super , 20);
	isMounted = 1;
	printf("Mounted: %s\n", imagefile);
	// this line is part of what I want to fix later
	// for unmount. leave here and read comment 
	// in umount.
	
	//fd = close(fd);
}
void showsuper(){
	if(isMounted == 0){
		printf("\nNo file mounted, cannot read superblock\n");
		return;
	}
	printf("\n\tnumber of inodes: %d", super.s_ninodes);
	printf("\n\tnumber of zones: %d", super.s_nzones);	
	printf("\n\tnumber of imap_blocks: %d", super.s_imap_blocks); 	      
        printf("\n\tnumber of zmap_blocks: %d", super.s_zmap_blocks);	
	printf("\n\tfirst data zone: %d", super.s_firstdatazone); 	
	printf("\n\tlog zone size: %d", super.s_log_zone_size);		
	printf("\n\tmax size: %d", super.s_max_size); 		
	printf("\n\tmagic: %d", super.s_magic); 			
	printf("\n\tstate: %d", super.s_state); 			
	printf("\n\tzones: %d\n", super.s_zones);			
}
// Unmounts specified image file

void miniumount(int numArgs,char *args[]){
	
	// Check to see if file is mounted	
	if(isMounted == 0){
		printf("\nNo file to unmount\n");
		return;	
	}
	
	char *imagefile;
	
	if (numArgs > 1){
        	imagefile = args[1];
    	}
	
	else{
        	return;
	}
	
	//Ran into problems here. it seems fd is flagged
	//for some reason and it wont let you unmount.
	//without this piece of code it works fine, but
	//would like to fix later for an extra check	
	
	// Check for valid file	
	/*fd = open(imagefile, O_RDONLY);
	
	if(fd == -1){
		printf("\nInvalid Input, file wasn't unmounted\n");
		return;	
	}*/
	
	
	//clear variables for superblock struct
	isMounted = 0;	
	super = (struct minix_super_block){0};
	printf("\nUnmounted: %s\n", imagefile);
	close(fd);
}
int main()
{
    createCommand("quit", &quit);
    createCommand("help", &help);
    createCommand("minimount", &minimount);
    createCommand("miniumount", &miniumount);
    createCommand("showzone", &showzone);
    createCommand("showsuper",&showsuper);
    createCommand("traverse", &traverse);
    do
    {
        prompt();
    } while (!stop);
    
    // Free memory used by linked list.
    struct command_node* cmd;
    while((cmd = commands))
    {
        commands = cmd->next;
        printf("free %s",cmd->name);
        free(cmd);
    }

    printf("\n");
    return 0;
}
