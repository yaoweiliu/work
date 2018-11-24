#include <stdio.h>

struct st
{
	int num;
	int (*view)(int);
};

static int test_view(int num)
{
	printf("view number is %d\n", num);
	return 0;
}

int main(int argc, char *arv[])
{
	char data[8] = "20181124";
	struct st test = {
		.num = 5,
		.view = test_view,
	};

	test.view(test.num);
	//printf("%p\n", test.view);
	printf("%s\n", data);

	return 0;
}