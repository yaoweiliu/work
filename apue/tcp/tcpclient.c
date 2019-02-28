#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "tcpclient.h"

static SOCKINFO *sinfo = NULL;

int create_pipe(SOCKINFO *sinfo)
{
	int ret;

	ret = pipe(sinfo->pipefd);
	if(ret) {
		perror("pipe()");
		return -1;
	}

	return 0;
}

void destory_pipe(SOCKINFO *sinfo)
{
	if(sinfo != NULL) {
		close(sinfo->pipefd[0]);
		close(sinfo->pipefd[1]);
	}
}

int create_socket(SOCKINFO *sinfo, const char *ip, unsigned int port)
{
	struct sockaddr addr;

	sinfo->sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sinfo->sfd == -1) {
		perror("socket()");
		return -1;
	}


}

void destory_socket(SOCKINFO *sinfo);
