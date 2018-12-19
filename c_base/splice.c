/*usage: cat splice.c |./splice out.log | cat */

#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char const *argv[])
{
	int fd;
	int len, slen;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd == -1) {
		perror("open");
		exit(1);
	}

	do {
		//tee stdin to stdout.
		len = tee(STDIN_FILENO, STDOUT_FILENO, INT_MAX, SPLICE_F_NONBLOCK);
		if(len < 0) {
			if(errno == EAGAIN)
				continue;
			perror("tee");
			exit(1);
		}
		else {
			if(len == 0)
				break;
		}

		//consume stdin by splicing it to a file.
		while(len > 0) {
			slen = splice(STDIN_FILENO, NULL, fd, NULL, len, SPLICE_F_MOVE);
			if(slen < 0) {
				perror("splice");
				break;
			}
			len -= slen;
		}
	} while(1);

	close(fd);
	
	exit(0);
}
