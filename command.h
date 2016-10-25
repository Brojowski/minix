#ifndef COMMAND_H
#define COMMAND_H


//Struct to assign variables to
//read from the superblock
struct super_block{

	unsigned short number_of_inodes;
	unsigned short number_of_blocks;
	unsigned short number_of_imap_blocks;
	unsigned short number_of_zmap_blocks;
	unsigned short first_zone_data;
	unsigned short log_zone_size;
	unsigned long max_size;
	unsigned short magic;
	unsigned short state;
	unsigned short zones;
};

void mount(char *imagefile);

#endif
