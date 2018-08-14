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
	int wd;

	fd = open(FILEPATH, O_RDWR);

	read(fd, &wd, sizeof(wd));

	close(fd);

	printf("the data from kernel is %d\n", wd);

	return 0;
}


