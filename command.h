#ifndef COMMAND_H
#define COMMAND_H


//Struct to assign variables to
//read from the superblock

struct minix_super_block {
    unsigned short s_ninodes;
    unsigned short s_nzones;
    unsigned short s_imap_blocks;
    unsigned short s_zmap_blocks;
    unsigned short s_firstdatazone;
    unsigned short s_log_zone_size;
    unsigned int s_max_size;
    unsigned short s_magic;
    unsigned short s_state;
    unsigned int s_zones;
};

struct minix_inode {
    unsigned short i_mode;
    unsigned short i_uid;
    unsigned int i_size;
    unsigned int i_time;
    unsigned char i_gid;
    unsigned char i_nlinks;
    unsigned short i_zone[9];
};

struct minix_dir_entry {
    unsigned short inode;
    char name[0];
};

struct minix_super_block super; 
struct minix_inode inode;

void minimount(int numArgs,char *args[]);
void miniumount(int numArgs,char *args[]);
void showsuper();

int isMounted;
int fd;

#endif
