/*------------------------------------------

filename: charge_prime.c
brief:    charge a number is or not prime from 1 to 100.
date:     2019-06-22
author:   yaowei.liu

------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE	200

static int charge_prime(int arr[])
{
	int i, j;

	for(i = 2; i < 201; i++) {
		//printf("%s\n", __func__);
		for(j = 2; j <= sqrt(i); j++) {
			if(i % j == 0) 
				break;
		}
		if(j > sqrt(i)) {
			arr[i] = i;
			printf("arr = %d\n", arr[i]);
		}
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	int array[SIZE] = {0};
	int i;

	charge_prime(array);

	for(i = 0; i < SIZE; i++) {
		if(array[i] != 0)
			printf("%d\n", array[i]);
	}

	return 0;
}




