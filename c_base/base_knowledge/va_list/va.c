#include <stdio.h>
#include <stdarg.h>

static float func(int num, ...) //第一个参数表示总参数的个数
{
	va_list vl;
	int sum = 0, i;

	va_start(vl, num);

	for(i = 0; i < num; i++) {
		//printf("arg = %d\n", va_arg(vl, int));
		sum += va_arg(vl, int);
	}

	va_end(vl);

	return sum/num*1.0;
}

int main(int argc, char const *argv[])
{
	float f;

	f = func(3, 1, 9, 50);
	printf("f = %.2f\n", f);

	return 0;
}

