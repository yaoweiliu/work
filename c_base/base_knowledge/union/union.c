#include <stdio.h>
#include <stdlib.h>

union Data
{
	int i;
	char c;
	float f;
};

int main(int argc, char const *argv[])
{
	union Data data;

	data.i = 10;
	data.c = 'l';
	data.f = 3.14;

	printf("data.i = %d, data.c = %c, data.f = %f\n", data.i, data.c, data.f);
	//data.i = 1078523331, data.c = �, data.f = 3.140000, union允许在相同的内存位置存储不同的数据类型,任何时候只能有一个成员带有值,同一时刻只能有一个成员有效

	data.i = 10;
	printf("\ndata.i = %d\n", data.i);
	data.c = 'l';
	printf("data.c = %c\n", data.c);
	data.f = 3.14;
	printf("data.f = %.2f\n", data.f);

	//大端模式，是指数据的高字节保存在内存的低地址中，而数据的低字节保存在内存的高地址中
	//小端模式，是指数据的高字节保存在内存的高地址中，而数据的低字节保存在内存的低地址中
	data.i = 0x1020; 
	printf("data.c = %0x\n", data.c);

	return 0;
}

