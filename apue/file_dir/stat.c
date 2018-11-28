#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	char *path = "/home/lyw/git/code/C/work";
	struct stat buf;

	if(stat(path, &buf) == -1) {
		perror("stat()");
		exit(-1);
	}

#ifdef DEBUG
	stat(pathname, &sb);
    if ((sb.st_mode & S_IFMT) == S_IFREG) {
        /* Handle regular file */
    }
#endif

    printf("file type: ");
    switch(buf.st_mode & S_IFMT) {
    case S_IFREG:
    	printf("regular file\n");
    	break;
    case S_IFBLK:
    	printf("block device\n");
    	break;
    case S_IFCHR:
    	printf("character device\n");
    	break;
    case S_IFDIR:
    	printf("directory\n");
    	break;
    case S_IFIFO:
    	printf("FIFO/pipe\n");
    	break;
    case S_IFLNK:
    	printf("symlink\n");
    	break;
    case S_IFSOCK:
    	printf("socket\n");
    	break;
    default:
    	printf("unknown\n");
    	break;
    }

    printf("Preferred I/O block size: %ld bytes\n", (long)buf.st_blksize);
    printf("File size: %lld bytes\n", (long long)buf.st_size);
    printf("Blocks allocated: %lld\n", (long long)buf.st_blocks);
    printf("Last status change: %s", ctime(&buf.st_ctime));

	return 0;
}