#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define	FILEPATH	"/dev/ts_misc_dev"

int main(void)
{
	int fd, ret;
	char wd = 97;

	fd = open(FILEPATH, O_RDWR);

	ret = fcntl(fd, F_GETFL);
	if(ret < 0) {
		perror("fcntl(1)");
		exit(1);
	}
	
	ret |= O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, ret);
	if(ret < 0) {
		perror("fcntl(2)");
		exit(1);
	}

	write(fd, &wd, sizeof(wd));

	close(fd);

	return 0;
}


