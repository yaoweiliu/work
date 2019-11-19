#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <poll.h>

#define UIO_DEV		"/dev/uio0"
#define MISC_DEV	"/dev/UIO_intr"

int main()
{
	struct pollfd fds;
	int fd, ret;
	int misc_fd, data;
	
	fd = open(UIO_DEV, O_RDWR);
	if (fd < 0) {
		printf("Error: Unable to open %s\n", UIO_DEV);
		return -1;
	}

	misc_fd = open(MISC_DEV, O_RDONLY);
	if(misc_fd < 0) {
		printf("open misc_dev error\n");
		close(fd);
		return -1;
	}
	
	while (1) {
		uint32_t info = 1;

		/* Enable Interrupt */
		info = 1;
		write(fd, &info, sizeof(info));
		
		fds.fd = fd;
		fds.events = POLLIN;
		ret = poll(&fds, 1, -1);

		/* Disable Interrupt */
		info = 0;
		write(fd, &info, sizeof(info));
		
		read(misc_fd, &data, sizeof(int));
		printf("read misc_data is %d\n", data);

		read(fd, &data, sizeof(int));
		printf("read irq_count is %d\n", data); //data is interrupt counts

		if (ret >= 1) {
			/* Handle Interrupt */
			printf("Interrup from kernel\n");
			sleep(1);
		} else {
			printf("Error: polling\n");
			close(fd);
			return -1;
		}
	}

	close(fd);

	return 0;

}
