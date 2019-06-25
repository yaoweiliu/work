#include <stdio.h>
#include <stdlib.h>

typedef struct data
{
	int a;
	char string[24];
}DATA;

int main(int argc, char const *argv[])
{
	int a = 0;
	int arr[2] = {0};
	DATA array[2] = {{1, "Yaowei.Liu"}, {2, "Liu Yaowei"}};

	printf("a = %p\n", &a);
	printf("arr = %p\n", arr);
	printf("arr = %p\n", &arr);

	for(a = 0; a < sizeof(array)/sizeof(array[0]); a++)
		printf("DATA: a = %d, string = %s\n", array[a].a, array[a].string);

	const char *str = "hello, i am const point.";
	printf("str is %s\n", str);

	/*
	str[1] = 'x'; //error: assignment of read-only location ‘*(str + 1u)’
	printf("str changed is %s\n", str);
	*/

	str = "hi, i am change."; //right. 不能通过常量指针修改指针指向的内容，但常量指针本身的指向可以改变。
	printf("str changed is %s\n", str);

	char str1[6] = "Hello";
	char str2[6] = "World";
	char *const p = str1;
	printf("p = %s\n", p);
	printf("p = %d\n", *p);//'h'--->104(ascii)

	/*
	p = str2; //error: assignment of read-only variable ‘p’
	*/

	*p='X'; //right. 指针常量不能改变指向，但是可以通过指针常量修改指针指向的内容。
	printf("%s\n",str1);
	
	return 0;
}


