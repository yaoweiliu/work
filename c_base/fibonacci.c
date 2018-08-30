/*1,1,2,3,5,8,13,21...*/

#include <stdio.h>

int fibonacci(int n)
{
	if(n == 0)
		return 0;
	if(n == 1)
		return 1;
	
	return (fibonacci(n-1) + fibonacci(n-2));
}

int main(void)
{
	int par = 50;
	long long result;

	result = fibonacci(par);

	printf("The last item is %lld\n", result);

	return 0;
}

