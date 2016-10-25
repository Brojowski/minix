#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#include "command.h"
#include "main.h"

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

void printArgs(char *args[])
{
    
}

/**
 * Purpose:
 *  1. Read user input.
 *  2. Parse command.
 *  3. TODO: Parse args.
 *  4. Find and run command.
 */
void prompt()
{
    char buf[MAX_INPUT_SIZE];
    write(STD_OUTPUT, PROMPT_SYMBOL, strlen(PROMPT_SYMBOL));
    int rawRead = read(STD_INPUT, buf, MAX_INPUT_SIZE);

    // Trim spaces that could cause errors.
    char *trimmed = trimwhitespace(buf);
    int left = strlen(trimmed);

    // If there are no spaces, the command
    // is the same as the trimmed command.
    char *cmdStr = trimmed;
    int cmdLen = left;
    int numWords = 1;
    for (int i = 0; i < left; i++)
    {
        if (trimmed[i] == ' ')
        {
            trimmed[i] = '|';
            numWords++;
        }
    }

    if (DEBUG)
    {
        printf("\t----Number of Words:%d\n",numWords);
        printf("\t----'%s'\n",trimmed);
    }

    char *args[numWords];
    int wordsFound = 0;
    cmdStr = args[0] = trimmed;
    for (int i = 0; i < left; i++)
    {
        printf("\t----%c\n",trimmed[i]);
        if (trimmed[i] == '|')
        {
            trimmed[i] = 0;
            args[wordsFound] = &trimmed[i+1];
            wordsFound++;
            if (DEBUG)
            {
                printf("\t----Char cutoff: %c\n", trimmed[i+1]);
                printf("\t----Word: %s\n",args[wordsFound]);
            }
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
            if (DEBUG)
            {
                printf("\t----RUN COMMAND\n");
            }
            //cmd->run();
            found = true;
        }
    }

    if (!found)
    {
        printf("%s: command not found\n", cmdStr);
    }

    // There seems to be a problem with data being
    // left in buf if 6+ chars were read. This messes
    // up the next command. Clearing the buffer seems
    // to fix this.
    for (int i = 0; i < rawRead; i++)
    {
        buf[i] = 0;
    }
}
// mount selected file, throw error if file is invalid.
// map superblock attributes to variables in the super_block struct.
// 

void mount(char *imagefile){
	int fd;
	if((fd = open(imagefile, O_RDONLY)) <= 0){
		printf("\nInvalid Input, file wasn't mounted");
	}
	else{
		struct super_block super;	
			
		fd = open(imagefile, O_RDONLY);
		lseek(fd, BLOCK_SIZE, SEEK_SET);
		read(fd, &super.number_of_inodes, 2);
		read(fd, &super.number_of_blocks, 2);
		read(fd, &super.number_of_imap_blocks, 2);
		read(fd, &super.number_of_zmap_blocks, 2);
		read(fd, &super.first_zone_data, 2);		
		read(fd, &super.log_zone_size, 2);
		// max size does not seem to be correct.
		// not sure why, as this should be the area to read it from.		
		read(fd, &super.max_size, 2);
		// not sure why magic number is 2 bytes further up
		// than it should be, it just is. 
		lseek(fd, 2, SEEK_CUR);
		read(fd, &super.magic, 2);
	//TODO state, zones. Will work on figuring this out
	}
}
int main()
{
    createCommand("quit", &quit);
    createCommand("help", &help);

    do
    {
        prompt();
    } while (!stop);

    printf("\n");
    return 0;
}
