#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define UIO_DEV		"/dev/uio0"
#define UIO_ADDR	"/sys/class/uio/uio0/maps/map0/addr"
#define UIO_SIZE	"/sys/class/uio/uio0/maps/map0/size"

static char uio_addr_buf[32];
static char uio_size_buf[32];

int main(int argc, char const *argv[])
{
	int ret;
	int uio_fd, addr_fd, size_fd;
	void *uio_addr, *access_address;
	int uio_size;

	uio_fd = open(UIO_DEV, O_RDWR);
	if(uio_fd < 0) {
		printf("%s: open uio_fd error\n", __func__);
		ret = uio_fd;
		goto uio_fd_error;
	}

	addr_fd = open(UIO_ADDR, O_RDONLY);
	if(addr_fd < 0) {
		printf("%s: open addr_fd error\n", __func__);
		ret = addr_fd;
		goto addr_fd_error;
	}

	size_fd = open(UIO_SIZE, O_RDONLY);
	if(size_fd < 0) {
		printf("%s: open size_fd error\n", __func__);
		ret = size_fd;
		goto size_fd_error;
	}

	read(addr_fd, uio_addr_buf, 32);
	read(size_fd, uio_size_buf, 32);

	uio_addr = (void *)strtoul(uio_addr_buf, NULL, 0);
	uio_size = (int)strtol(uio_size_buf, NULL, 0);

	access_address =  mmap(NULL, uio_size, PROT_READ | PROT_WRITE, MAP_SHARED, uio_fd, 0);
	if(access_address == MAP_FAILED) {
		printf("%s: mmap error: %s errno %d\n", __func__, strerror(errno), errno);
		goto mmap_error;
	}

	printf("Device Address %p (length: %d) virutal address: %p\n",\
		uio_addr, uio_size, access_address);

	munmap(access_address,uio_size);

	close(size_fd);
	close(addr_fd);
	close(uio_fd);

	return 0;

mmap_error:
	close(size_fd);
size_fd_error:
	close(addr_fd);
addr_fd_error:
	close(uio_fd);
uio_fd_error:
	return ret;
}

