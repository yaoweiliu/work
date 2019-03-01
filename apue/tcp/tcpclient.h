#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

typedef void (*cb_func_t)(void *buff, unsigned int len);

typedef struct sock_fd
{
	int sfd;
	int pipefd[2];
	cb_func_t cb;
}SOCKINFO;

int create_pipe(SOCKINFO *sinfo);

void destory_pipe(SOCKINFO *sinfo);

int create_socket(SOCKINFO *sinfo, const char *ip, unsigned int port);

void destory_socket(SOCKINFO *sinfo);

void set_callback_func(SOCKINFO *sinfo, cb_func_t callback, unsigned int len);

int select_handle(SOCKINFO *sinfo);

SOCKINFO *start_thread_handle(void);

int signal_handle(void);

#endif

