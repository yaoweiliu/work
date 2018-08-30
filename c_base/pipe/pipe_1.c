#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	SIZE	11

static int fd[2];

static void wake_up()
{
	int ret;
	close(fd[0]);
	do {
		write(fd[1], "x", 1);
	}while(ret < 0);
}

int main(void)
{
	int ret;
	pid_t pid;
	char buf[SIZE] = {0};

	ret = pipe(fd);
	if(ret) {
		perror("pipe()");
		exit(1);
	}

	pid = fork();
	if(pid < 0) {
		perror("fork()");
		exit(1);
	}
	else if(pid == 0) {
		wake_up();
	}

	while(1) {
		close(fd[1]);
		read(fd[0], buf, 1);
		if(buf) {
			fprintf(stdout, "%s\n", buf);
			break;
		}
	}

	return 0;
}

