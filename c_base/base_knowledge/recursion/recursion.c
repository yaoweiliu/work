#include <stdio.h>
#include "base.h"

/* 4! = 1*2*3*4 */
double calc_data(int i)
{
	if(i <= 1)
		return 1;
	
	printf("i = %d\n", i);
	return i*calc_data(i-1);
}

