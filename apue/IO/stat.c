#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *charge_file_type(const struct stat *buf)
{
	char *ptr = NULL;

	if(S_ISREG(buf->st_mode))
		ptr = "It is a regular file.";
	else if(S_ISDIR(buf->st_mode))
		ptr = "It is a directory.";
	else
		ptr = "Unknow.";

	return ptr;
}

int charge_file_size(const struct stat *buf)
{
	return buf->st_size;
}

int main(int argc, char const *argv[])
{
	struct stat buf;
	char *info = NULL;
	int size;

	stat("./isdigit.c", &buf);
	info = charge_file_type(&buf);
	size = charge_file_size(&buf);

	printf("%s\n", info);
	printf("%d\n", size);

	return 0;
}