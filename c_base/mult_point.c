#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int a = 5;
	int *p = NULL;
	int **p_2 = NULL;
	int ***p_3 = NULL;

	p = (int *)malloc(sizeof(int));
	printf("p    : %p\n", p);//一级指针

	p_2 = &p;
	printf("p_2  : %p\n", p_2);//二级指针
	printf("*p_2 : %p\n", *p_2);//一级指针

	p_3 = &p_2;
	printf("p_3  : %p\n", p_3);//三级指针
	printf("*p_3 : %p\n", *p_3);//二级指针
	printf("**p_3: %p\n", **p_3);//一级指针

	return 0;
}