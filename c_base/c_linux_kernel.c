#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct
{
	int a;
	char b;
	void (*con)(int, char);
}DATA_T;

static void test_view(int a, char c)
{
	printf("a = %d, c = %c\n", a, c);

	return ;
}

int main(int argc, char const *argv[])
{
	int *cc = NULL;
	DATA_T *data;

	cc = malloc(sizeof(int *));
	if(cc == NULL)
	{
		fprintf(stderr, "no memory!\n");
		return -1;
	}

	/*a is first member of DATA_T*/
	printf("before data's address is: %p\n", data);
	printf("before cc's address is: %p\n", cc);
	data = container_of(cc, DATA_T, a);
	printf("after data's address is: %p\n", data);
	printf("after cc's address is: %p\n", cc);
	
	data->a = 5;
	data->b = 'l';
	data->con = test_view;
	test_view(data->a, data->b);

	return 0;
}

