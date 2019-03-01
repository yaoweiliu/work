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
	char buf[19];
	int ret;

	//ret = (sinfo->pipefd[0] > sinfo->pipefd[1]) ? sinfo->pipefd[0] : sinfo->pipefd[1];
	max_fd = (sinfo->pipefd[0] > sinfo->sfd) ? sinfo->pipefd[0] : sinfo->sfd;

	FD_ZERO(&readfds);
	memset(buf, '0', 19);

	while(1) {
		FD_SET(sinfo->sfd, &readfds);
		FD_SET(sinfo->pipefd[0], &readfds);

		select(max_fd+1, &readfds, NULL, NULL, NULL);
		if(FD_ISSET(sinfo->sfd, &readfds)) {
			//recv and recv callback. TODO
			recv(sinfo->sfd, buf, 19, 0);
			printf("%s: recv msg is %s.\n", __func__, buf);
		}

		if(FD_ISSET(sinfo->pipefd[0], &readfds)) {
			//read and send. TODO
			read(sinfo->pipefd[0], buf, 19);
			ret = send(sinfo->sfd, buf, strlen(buf), 0);
			if(ret == -1) {
		    	perror("send()");
		    	return -1;
    		}
			printf("%s: pipe ready, data is %s\n", __func__, buf);
			break;
		}
	}

	return 0;
}

void *start_select(void *sinfo)
{
	select_handle(sinfo);

	pthread_exit(NULL);
}

int start_thread_handle(SOCKINFO *sinfo)
{
	int ret;
	//pthread_t pid;

	ret = pthread_create(&sinfo->pid, NULL, start_select, (void *)sinfo);
	if(ret) {
		fprintf(stderr, "pthread_create error.\n");
		return -1;
	}

	//pthread_join(pid, NULL);

	return 0;
}

SOCKINFO *create_socket_info(void)
{
	int ret;
	SOCKINFO *sinfo = NULL;

	sinfo = malloc(sizeof(SOCKINFO));
	if(!sinfo) {
		fprintf(stderr, "malloc error.\n");
		goto malloc_error;
	}

	ret = create_socket(sinfo, "127.0.0.1", 6666);
	if(ret) {
		fprintf(stderr, "create_socket error.\n");
		goto socket_error;
	}

	ret = create_pipe(sinfo);
	if(ret) {
		fprintf(stderr, "create_pipe error.\n");
		goto pipe_error;
	}

	return sinfo;

thread_error:
	destory_pipe(sinfo);
pipe_error:
	destory_socket(sinfo);
socket_error:
	free(sinfo);
malloc_error:
	return NULL;
}

void *write_pipe(void *sinfo)
{
	/*test pipe demo code*/
	//close(sinfo->pipefd[0]);

	SOCKINFO *info = (SOCKINFO *)sinfo;
	write(info->pipefd[1], "test pipe is ready.", 19);

	pthread_exit(NULL);

	return NULL;
}

/*test code*/
int main(int argc, char const *argv[])
{
	int ret;
	SOCKINFO *sinfo;
	//pthread_t pipe_fd;

	sinfo = create_socket_info();

	start_thread_handle(sinfo);

	ret = pthread_create(&sinfo->pipe_fd, NULL, write_pipe, (void *)sinfo);
	if(ret) {
		fprintf(stderr, "pthread_create error.\n");
		goto thread_error;
	}

	pthread_join(sinfo->pipe_fd, NULL);
	pthread_join(sinfo->pid, NULL);

	destory_pipe(sinfo);
	destory_socket(sinfo);
	free(sinfo);
	sinfo = NULL;
	
	return 0;
thread_error:
	destory_pipe(sinfo);
	destory_socket(sinfo);
	free(sinfo);
	sinfo = NULL;

	return -1;
}


