/*1,1,2,3,5,8,13,21...*/

#include <stdio.h>
#include <stdlib.h>

unsigned int fibonacci(int num)
{
	unsigned int curr_item, pre_item, next_item;

	curr_item = pre_item = 1;
	while(num > 2) {
		num -= 1;
		next_item = pre_item;
		pre_item = curr_item;
		curr_item = pre_item + next_item;
	}

	return curr_item;
}

int main(int argc, char const *argv[])
{
	int num;

	printf("input a number: ");
	scanf("%d", &num);

	printf("item is %u\n", fibonacci(num));

	return 0;
}

