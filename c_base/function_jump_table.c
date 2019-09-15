//uboot/common/board_f.c/board_init_f()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef double (*func_sequence_t)(double, double);

static double _add(double a, double b)
{
	return a+b;
}

static double _sub(double a, double b)
{
	return a-b;
}

static double _mul(double a, double b)
{
	return a*b;
}

static double _div(double a, double b)
{
	return b ? (a/b) : -1.0;
}

static const func_sequence_t func_sequence[] = 
{
	_add,
	_sub,
	_mul,
	_div,
	NULL,//以NULL作为结尾，否则遍历时数据越界
};

static int run_func_list(const func_sequence_t func_sequence_list[])
{
	const func_sequence_t *func = NULL;

	for(func = func_sequence_list; *func; ++func) {
		if(*func)
			printf("result is %.1f\n", (*func)(3.0, 4.0));
		else
			return -1;
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	/* 1.
	func_sequence_t func = NULL;

	for(int i = 0; i < sizeof(func_sequence)/sizeof(func_sequence_t); i++) {
		func = func_sequence[i];
		if(func)
			printf("result is %.1f\n", func((double)i, (double)(i+1)));
	}
	*/

	//2.
	run_func_list(func_sequence);

	return 0;
}



