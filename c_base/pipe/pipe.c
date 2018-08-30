#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	SIZE	11

int main(void)
{
	int ret;
	int fd[2];
	pid_t pid;
	char buf[SIZE];

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
		close(fd[0]);
		write(fd[1], "hello world", SIZE);
	}
	else {
		close(fd[1]);
		read(fd[0], buf, SIZE);
		puts(buf);
	}

	return 0;
}

