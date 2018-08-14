#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define	FILEPATH	"/dev/ts_misc_dev"

int main(void)
{
	int fd;
	char wd = 97;

	fd = open(FILEPATH, O_RDWR);

	write(fd, &wd, sizeof(wd));

	close(fd);

	return 0;
}


