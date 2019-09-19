#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	int fd;

	fd = open("/dev/timer_demo", O_RDWR);
	if(fd < 0) {
		printf("open error\n");
		return -1;
	}

	write(fd, "hello", 6);

	close(fd);

	return 0;
}