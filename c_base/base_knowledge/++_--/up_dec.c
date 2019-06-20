/*----------------------------------
brief: i = ++a;--->先自增，再赋值;
	   i = a++;--->先赋值，再自增;
	   --: 同上
date: 2019.06.20
----------------------------------*/

#include <stdio.h>

int main(int argc, char const *argv[])
{
	int i = 0, a = 5;

	i = a++;
	printf("i = %d, a = %d\n", i, a); //i = 5, a = 6;

	i = ++a;
	printf("i = %d, a = %d\n", i, a); //i = 7, a = 7;

	i = a--;
	printf("i = %d, a = %d\n", i, a); //i = 7, a = 6;

	i = --a;
	printf("i = %d, a = %d\n", i ,a); //i = 5, a = 5;

	i = (i == a ? 1 : 0);
	printf("i = %d\n", i); //i = 1;

	return 0;
}