#ifndef __TINYFS_H__
#define __TINYFS_H__

#define MAXLEN	8
#define	MAX_FILES	32
#define MAX_BLOCKSIZE	512

struct dir_entry
{
	char filename[MAXLEN];
	uint8_t idx;
};

struct file_blk
{
	uint8_t busy;
	mode_t mode;
	uint8_t idx;
	union {
		uint8_t file_size;
		uint8_t dir_chilren;
	};
	char data[0];
};


