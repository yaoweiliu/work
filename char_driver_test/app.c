#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(void)
{
	int fd;
	int val = 0;
	int buf[1] = {0};

	printf("val is %d\n", val);
	
	fd = open("/dev/test", O_RDWR);
	if(fd < 0) {
		printf("fd error %d\n", fd);
		return -1;
	}

	if(ioctl(fd, 101, &val))
		printf("ioctl failed\n");
	printf("val is %d\n", val);

	read(fd, buf, 4);
	printf("buf[0] = %d\n", buf[0]);

	close(fd);

	return 0;
}
