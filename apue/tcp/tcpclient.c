#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
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
	struct sockaddr_in dst_addr;
    struct hostent *hostname;
    int ret;
    const char *buf = "create socket success.";

	sinfo->sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sinfo->sfd == -1) {
		perror("socket()");
		return -1;
	}

	hostname = gethostbyname(ip);

    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(port);
    dst_addr.sin_addr = *((struct in_addr *)hostname->h_addr);
    memset(&(dst_addr.sin_zero), 0, sizeof(dst_addr.sin_zero));

    ret = connect(sinfo->sfd, (struct sockaddr *)&dst_addr, sizeof(struct sockaddr));
    if(ret) {
    	perror("connect()");
    	return -1;
    }

    ret = send(sinfo->sfd, buf, strlen(buf), 0);
    if(ret == -1) {
    	perror("send()");
    	return -1;
    }

    return 0;
}

void destory_socket(SOCKINFO *sinfo)
{
	if(sinfo != NULL) {
		close(sinfo->sfd);
	}
}

void set_callback_func(SOCKINFO *sinfo, cb_func_t callback, unsigned int len)
{
	sinfo->cb = callback;
}

int select_handle(SOCKINFO *sinfo)
{
	int max_fd;
	fd_set readfds;

	//ret = (sinfo->pipefd[0] > sinfo->pipefd[1]) ? sinfo->pipefd[0] : sinfo->pipefd[1];
	max_fd = (sinfo->pipefd[0] > sinfo->sfd) ? sinfo->pipefd[0] : sinfo->sfd;

	FD_ZERO(&readfds);

	while(1) {
		FD_SET(sinfo->sfd, &readfds);
		FD_SET(sinfo->pipefd[0], &readfds);

		select(max_fd+1, &readfds, NULL, NULL, NULL);
		if(FD_ISSET(sinfo->sfd, &readfds)) {
			//recv and recv callback. TODO
		}

		if(FD_ISSET(sinfo->pipefd[0], &readfds)) {
			//read and send. TODO
		}
	}
}

void *start_select(void *sinfo)
{
	select_handle(sinfo);

	pthread_exit(NULL);
}

int start_thread_handle(void)
{
	int ret;
	pthread_t pid;
	SOCKINFO *sinfo = NULL;

	sinfo = malloc(sizeof(SOCKINFO));
	if(!sinfo) {
		fprintf(stderr, "malloc error.\n");
		goto malloc_error;
	}

	ret = create_socket(sinfo, "192.168.1.155", 6666);
	if(ret) {
		fprintf(stderr, "create_socket error.\n");
		goto socket_error;
	}

	ret = create_pipe(sinfo);
	if(ret) {
		fprintf(stderr, "create_pipe error.\n");
		goto pipe_error;
	}

	ret = pthread_create(&pid, NULL, start_select, (void *)sinfo);
	if(ret) {
		fprintf(stderr, "pthread_create error.\n");
		goto thread_error;
	}

	pthread_join(pid, NULL);

	return 0;

thread_error:
	destory_pipe(sinfo);
pipe_error:
	destory_socket(sinfo);
socket_error:
	free(sinfo);
malloc_error:
	return -1;
}

/*test code*/
int main(int argc, char const *argv[])
{
	int ret;

	ret = start_thread_handle();
	
	return 0;
}


