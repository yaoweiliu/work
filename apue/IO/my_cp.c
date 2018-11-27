#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 	100

int main(int argc, char const *argv[])
{
	int sfd, dfd;
	ssize_t read_len, write_len;
	char buf[SIZE] = {"0"};
	int pos;

	if(argc < 3) {
		printf("Usag: my_cp source_file dect_file\n");
		exit(-1);
	}

	//sfd = open("/etc/sysctl.conf", O_RDONLY);
	sfd = open(argv[1], O_RDONLY);
	if(sfd < 0) {
		perror("open()");
		exit(-1);
	}

	//dfd = open("./cp.txt", O_RDWR | O_CREAT);//O_APPEND
	dfd = open(argv[2], O_RDWR | O_CREAT);
	if(dfd < 0) {
		perror("open()");
		close(sfd);
		exit(-1);
	}

	while((read_len = read(sfd, buf, SIZE)) > 0) {
		pos = 0;
		write_len = write(dfd, buf+pos, read_len);
		pos += write_len;
	}

	close(sfd);
	close(dfd);

	return 0;
}