#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

typedef struct sock_fd
{
	int sfd;
	int pipefd[2];
}SOCKINFO;

int create_pipe(SOCKINFO *sinfo);

void destory_pipe(SOCKINFO *sinfo);

int create_socket(SOCKINFO *sinfo, const char *ip, unsigned int port);

void destory_socket(SOCKINFO *sinfo);

#endif

