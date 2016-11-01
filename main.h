#ifndef MAIN_HEAD
#define MAIN_HEAD

#define BLOCK_SIZE 1024

#define MAX_INPUT_SIZE 100
#define STD_OUTPUT 1
#define STD_INPUT 0
#define PROMPT_SYMBOL "\nminix: "

// Define bool for ease of use.
#define bool char
#define false 0
#define true 1

// Save functions for commands.
typedef void (*commandFunction)(int numArgs, char *args[]);

struct command_node
{
    struct command_node *next;
    char * name;
    // Ints are more efficient to compare.
    // Compare string size before doing strcmp.
    int cmdLen;
    commandFunction run;
};

struct command_node *commands;
bool stop;

bool DEBUG;

#endif