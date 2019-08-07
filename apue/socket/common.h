/**********************
 * brief: common.h
 *
 *
 *********************/

#ifndef __COMMON_H__
#define __COMMON_H__

#define BUFLEN	1024
#define DEFAULT_PORT 8000 

struct socket_file
{
	int sockfd;
	int new_fd;
	char buf[BUFLEN];
};

#endif

