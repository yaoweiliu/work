#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	FILE *fp = NULL;
	char *buf = NULL;
	size_t n = 0;
	ssize_t ret = 0;

	fp = fopen("/etc/sysctl.conf", "r");
	if(!fp) {
		perror("fopen()");
		exit(-1);
	}

	while((ret = getline(&buf, &n, fp)) != -1) {
		printf("Retrieved line of length %zu\n", ret);//ret is lenth of each line
		printf("%s\n", buf);
	}
	
	/*
	ret = getline(&buf, &n, fp);
	if(ret != -1) {
		printf("Retrieved line of length %zu\n", ret);
		printf("%s\n", buf);
	}
	*/

	free(buf);
	fclose(fp);

	return 0;
}