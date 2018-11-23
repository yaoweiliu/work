#include <stdio.h>

int main(int argc, char const *argv[])
{
	unsigned int data = 1024;
	unsigned int val = 255;

	printf("before data: %u\n", data);
	printf("before val: %u\n", val);

	data |= (1 << 6);

	printf("after data: %u\n", data);

	val &= ~(1 << 6);

	printf("after val: %u\n", val);

	return 0;
}