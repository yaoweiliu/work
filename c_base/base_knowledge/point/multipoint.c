#include <stdio.h>

int main(int argc, char const *argv[])
{
	int a = 10;
	int b = 20;
	int *p = &a;
	int **ptr = &p;

	printf("a = %d\n", a);
	printf("a = %d\n", *p);
	printf("a = %d\n", **ptr);

	printf("p = %p\n", &a);
	printf("p = %p\n", p);
	printf("p = %p\n", *ptr); //二级指针存储的是一级指针的地址

	*ptr = &b; //使用二级指针改变一级指针的指向
	printf("p = %p\n", p);
	printf("p = %p\n", *ptr);
	printf("*p = %d\n", *p);
	printf("**ptr = %d\n", **ptr);

	return 0;
}


