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

struct minix_super_block super; 

void mount(int numArgs,char *args[]);
void showsuper();

#endif
