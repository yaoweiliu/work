#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *get_rand(void)
{
	static int r[10];
	int i = 0;

	srand((unsigned int)time(NULL));
	for(i = 0; i < 10; i++) {
		r[i] = rand();
		printf("r[%d] = %d\n", i, r[i]);
	}

	return r;
}

int main(int argc, char const *argv[])
{
	int i = 0;
	int *p = get_rand();

	for(i = 0; i < 10; i++)
		printf("%d\n", *(p++));

	return 0;
}

