#include "switch.h"
#include <stdio.h>
#include <stdlib.h>

static int _get_input_year(void)
{
	printf("请输入年数: \n");
	scanf("%d", &year);

	printf("请输入月数: \n");
	scanf("%d", &month);

	return 0;
}

static int _calc_leap_year(int _month)
{	
	int flag;
	if((year%4==0 && year%100!=0) || (year%400)==0)
		flag = 1;
	else
		flag = 0;

	switch(_month)
	{
	case 1:
		break;
	case 2:
		if(flag)
			printf("29\n");
		else
			printf("28\n");
		break;
	default:
		break;
	}

	return 0;
}

static struct callback_func switch_test = 
{
	.get_input_year = _get_input_year,
	.calc_leap_year = _calc_leap_year,
};

int main(int argc, char const *argv[])
{
	/* code */

	switch_test.get_input_year();
	switch_test.calc_leap_year(month);

	return 0;
}


