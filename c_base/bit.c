#include <stdio.h>

int main(int argc, char const *argv[])
{
	unsigned int data = 1024;
	unsigned int val = 255;
	unsigned int ret = 63;

	printf("before data: %u\n", data);
	printf("before val: %u\n", val);
	printf("before ret: %u\n", ret);

	data |= (1 << 6);
	val &= ~(1 << 6);
	ret |= (3 << 6);

	printf("after data: %u\n", data);
	printf("after val: %u\n", val);
	printf("after ret: %u\n", ret);

	return 0;
}