/*----------------------------------------------------------------------------------
为了节省存储空间，并使处理简便，C 语言又提供了一种数据结构，称为"位域"或"位段"。
所谓"位域"是把一个字节中的二进位划分为几个不同的区域，并说明每个区域的位数。
每个域有一个域名，允许在程序中按域名进行操作。这样就可以把几个不同的对象用一个字节的二进制位域来表示。

一个位域存储在同一个字节中，如一个字节所剩空间不够存放另一位域时，则会从下一单元起存放该位域.
由于位域不允许跨两个字节，因此位域的长度不能大于一个字节的长度，也就是说不能超过8位二进位.
----------------------------------------------------------------------------------*/

#include <stdio.h>

struct data
{
	unsigned int a:3;
	char b:1;
	int c:3; //(-4):(3)
};

/* 结构体的总大小为结构体最宽基本类型成员大小的整数倍，如有需要编译器会在成员末尾加上填充字节 */
struct mem_1
{
	unsigned char c; //存储空间分布为c(1byte)+空闲(3byte)+d(4byte)+b(1byte)+空闲(3byte)=12(byte)。
	unsigned int d;
	unsigned char b;
};

struct mem_2
{
	unsigned int a; //存储空间分布为a(4byte)+c(1byte)+b(1byte)+空闲(2byte)=8(byte)。
	unsigned char c;
	unsigned char b;
};

int main(int argc, char const *argv[])
{
	struct data dt = {.a = 5, .b = 0, .c = 5,};

	printf("dt.a = %d, dt.b = %d, dt.c = %d\n", dt.a, dt.b, dt.c);

	struct mem_1 m1;
	struct mem_2 m2;

	printf("m1 memory is %lu, m2 memory is %lu\n", sizeof(m1), sizeof(m2));

	return 0;
}

