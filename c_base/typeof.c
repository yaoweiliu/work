#include <stdio.h>

int main(void)
{
	int a = 10;
	int b = 
	({
	typeof(a) _a = 50;
	typeof(a) _b = 70;
	_a;
	_b;
	});

	printf("b = %d\n", b);

	return 0;
}
