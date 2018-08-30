#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define	OPTION		"a:-l"
#define FILEPATH	"/home/lyw/test/au/glob.c"

int main(int argc, char *argv[])
{
	int ret = 0;
	struct stat buf;

	if(argc < 2) {
		fprintf(stderr, "Usage is %s + filename!\n", argv[0]);
		exit(1);
	}

	if(stat(FILEPATH, &buf)) {
		perror("stat()");
		exit(1);
	}

	while(ret != -1) {
		ret = getopt(argc, argv, OPTION);
		switch(ret) {
			case -1:
				printf("Parse Done!\n");
				break;
			case 'a':
				printf("%d\n", (int)buf.st_uid);
				break;
			default:
				break;
		}
	}
	
	return 0;
}
