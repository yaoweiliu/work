#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc, char const *argv[])
{
	pid_t pid;

	pid = fork();
	if(pid < 0)
		exit(1);
	else if(pid == 0) {
		execle("/bin/ls", "/bin/ls", NULL, environ);
		printf("no return.\n");
		exit(0);
	}

	printf("has return.\n");

	wait(NULL);

	return 0;
}