#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
	pid_t pid;

	fflush(NULL);

	pid = fork();
	if(pid < 0) {
		perror("fork()");
		exit(1);
	}
	else if(pid == 0) {
		printf("[%d] chird.\n", getpid());
		exit(0);
	}
	else {
		printf("[%d] parent.\n", getpid());
	}

	sleep(5);

	return 0;
}