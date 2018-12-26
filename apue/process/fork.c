#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	pid_t pid;
	int i;

#if 0
	for(i = 0; i < 10; i++) {
		fflush(NULL);
		pid = fork();
		if(pid < 0) {
			perror("fork()");
			exit(1);
		}
		else if(pid == 0) {
			//TODO. dosomething use exec.
			printf("[%d] chird.\n", getpid());
			exit(0);
		}
	}

	for(i = 0; i < 10; i++)
		wait(NULL);

#endif

	fflush(NULL);

	pid = fork();
	if(pid < 0) {
		perror("fork()");
		exit(1);
	}
	else if(pid == 0) {
		//execl("/bin/date", "date", "+%s", NULL);
		execl("/bin/date", "date", NULL);
		printf("no return.\n");
		perror("execl()");
		exit(1);
	}
	else {
		wait(NULL);
	}
	
	return 0;
}