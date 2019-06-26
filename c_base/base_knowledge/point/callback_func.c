#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int (*function)(void);

static int get_rand(void)
{
	//srand((unsigned int)time(NULL));

	return rand(); 
}

static void get_array(int *arr, int size, function func)
{
	int i = 0;

	for(i = 0; i < size; i++)
		*(arr++) = func();
}

int main(int argc, char const *argv[])
{
	int array[10];
	int i;

	get_array(array, 10, get_rand);

	for(i = 0; i < 10; ++i)
		printf("array[%d] = %d\n", i, array[i]);

	return 0;
}


