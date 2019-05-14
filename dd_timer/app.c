#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEVNAME	"/dev/wait"

int main(int argc, char const *argv[])
{
	int fd, ret;
	fd_set r_set;
	unsigned long buf[1];

	fd = open(DEVNAME, O_RDONLY | O_NONBLOCK);

	printf("1: %d\n", fd);
	FD_ZERO(&r_set);
	FD_SET(fd, &r_set);

	while(1) {
		ret = select(fd+1, &r_set, NULL, NULL, NULL);

		//printf("2: %d\n", ret);
		if(ret == -1)
			printf("%s: select error\n", __func__);
		else if(ret > 0) {
			if(FD_ISSET(fd, &r_set)) {
				read(fd, buf, sizeof(buf));
				printf("buf[%d]: %lu\n", 0, buf[0]);
			}
		}
	}

	close(fd);

	return 0;
}