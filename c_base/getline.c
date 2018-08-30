#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	FILE *fp = NULL;
	char *line = NULL;
	size_t n = 0;

	fp = fopen("./elevinfo.data", "r+");
	if(fp == NULL) {
		perror("fopen()");
		exit(1);
	}

	getline(&line, &n, fp);

	printf("line = %s", line);

	free(line);

	return 0;
}
