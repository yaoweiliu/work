/* 
 * stat() getopt() glob()
 * my_ls -a -l
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int test_glob(glob_t *p_glob)
{
	return glob("*", GLOB_NOSORT, NULL, p_glob);
}

void print_file_type(const struct stat *buf)
{
	char info[11] = "----------";

	/*
	S_ISREG(m)  is it a regular file?

	S_ISDIR(m)  directory?

	S_ISCHR(m)  character device?

	S_ISBLK(m)  block device?

	S_ISFIFO(m) FIFO (named pipe)?

	S_ISLNK(m)  symbolic link?  (Not in POSIX.1-1996.)

	S_ISSOCK(m) socket?  (Not in POSIX.1-1996.)
	*/

	if(S_ISREG(buf->st_mode))
		strncpy(info, "----------", 10);
	else if(S_ISDIR(buf->st_mode))
		strncpy(info, "d---------", 10);

	printf("%s\n", info);
}

int get_file_info(const char *path)
{
	struct stat buf;

	if(stat(path, &buf)) {
		perror("stat()");
		return -1;
	}

	print_file_type(&buf);

	return 0;
}

int main(int argc, char const *argv[])
{
	glob_t p_glob;
	int ret, i;

	if(argc < 3) {
		fprintf(stderr, "Usage: %s -a -l\n", argv[0]);
		return -1;
	}

	ret = test_glob(&p_glob);
	if(ret) {
		fprintf(stderr, "%s\n", "glob() error");
		return -1;
	}

	for(i = 0; i < p_glob.gl_pathc; i++) {
		printf("%s\n", p_glob.gl_pathv[i]);
		ret = get_file_info(p_glob.gl_pathv[i]);
		if(ret) {
			printf("%s\n", "stat error");
			break;
		}
	}

	globfree(&p_glob);

	return 0;
}


