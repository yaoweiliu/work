#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef int (*func)(int a, int b);

double _add(double a, double b)
{
	return a+b;
}
double _sub(double a, double b)
{
	return a-b;
}
double _mul(double a, double b)
{
	return a*b;
}
double _div(double a, double b)
{
	if(b)
		return a/b;
}

double (*oper_func[])(double, double) = {_add, _sub, _mul, _div};

int max(int a, int b)
{
	return a > b ? a : b;
}

int callback(int a, int b, func f)
{
	if(!f)
		return -1;
	return f(a, b);
}

int main(int argc, char const *argv[])
{
	#if 0
	char c;

	printf("Please input a oper: ");
	scanf("%c", &c);

	switch (c) {
	case '+':
		add()
	}
	#endif
	printf("max is %d\n", callback(12, 10, max));

	for(int i=0; i<4; i++) {
		printf("result is %.2f\n", oper_func[i](10, 2));
	}

	return 0;
}
