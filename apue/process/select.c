#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define print_error(msg)	\
		do { perror("msg"); exit(1); } while(0)

#define print_info(msg)		\
		do { printf("%s\n", msg); exit(0); } while(0)

int main(int argc, char const *argv[])
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	FD_ZERO(&rfds);//init rfds.
	FD_SET(0, &rfds);//add stdin to rfds.

	//wait up to five seconds.
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	retval = select(1, &rfds, NULL, NULL, &tv);

	if(retval == -1) {
		print_error("select()");
	}
	else if(retval) {
		printf("Data is available now.\n");
		//print_info("Data is available now.");
	}
	else
		printf("No data within five seconds.\n");

	return 0;
}
