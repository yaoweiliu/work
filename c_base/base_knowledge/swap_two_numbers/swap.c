/*----------------------------
brief: swap two numbers
date:  2019-06-22
author:yaowei.liu
----------------------------*/

#include <stdio.h>

void swap(int *a, int *b)
{
	/*
	int t;

	t = *a;
	*a = *b;
	*b = t;
	*/

	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

int main(int argc, char const *argv[])
{
	int a = 10, b = 20;

	printf("before: a = %d, b = %d\n", a, b);
	swap(&a, &b);
	printf("after: a = %d, b = %d\n", a, b);

	return 0;
}


