#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PRINT_WITH_SOMETHING(format, ...)	\
{											\
	printf(""format"\n", ##__VA_ARGS__);	\
}								

static void alarm_cb(int arg)
{
	PRINT_WITH_SOMETHING("signal coming...");
}

int main(int argc, char const *argv[])
{
	signal(SIGALRM, alarm_cb);
	alarm(5);
	sleep(6);

	return 0;
}