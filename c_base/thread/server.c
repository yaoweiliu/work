#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static uint16_t portt = 2088;
#define SIZE	5

static void *thread_start(void *data)
{
	int ret;
	int sd, new_sd;
	struct in_addr iaddr;
	struct sockaddr_in addr;
	char buf[SIZE];

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0) {
		perror("socket() is error!");
		exit(1);
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(portt);
	
	int count = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &count, sizeof(count));

	ret = bind(sd, (const struct sockaddr *)&addr, sizeof(addr));
	if(ret) {
		perror("bind() is error!");
		exit(1);
	}

	ret = listen(sd, 10);
	if(ret) {
		perror("listen() is error!");
		exit(1);
	}

	while(1) {
		new_sd = accept(sd, NULL, NULL);
		if(new_sd == -1) {
			perror("accept() is error!");
			exit(1);
		}

		dup2(new_sd, 1);
		printf("Welcom use this telnet debug!\n");
		fflush(stdout);
		ret = recv(new_sd, buf, SIZE, 0);
		if(ret == -1) {
			perror("recv() is error!");
			exit(1);
		}
		if(0 == strncmp(buf, "exit", 4)) {
			printf("Now will exit this mode!\n");
			fflush(stdout);
			break;
		}
		else if(!strncmp(buf, "help", 4)) {
			printf("The usage is...\n");
			fflush(stdout);
		}
		else {
			printf("no this case!\n");
			fflush(stdout);
			break;
		}
		//shutdown(new_sd, SHUT_RDWR);
		close(new_sd);
	}

	close(sd);
}

int main(int argc, char *argv[])
{
	int ret;
	pthread_t pid;

	ret = pthread_create(&pid, NULL, thread_start, NULL);
	if(ret) {
		fprintf(stderr, "pthread_create is error\n");
		exit(1);
	}

	ret = pthread_join(pid, NULL);
	if(ret) {
		fprintf(stderr, "pthread_join is error\n");
		exit(1);
	}

	return 0;
}



