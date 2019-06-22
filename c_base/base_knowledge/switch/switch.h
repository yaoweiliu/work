/*----------------------------------------

filename: switch.h
brief:    switch...case
date:	  2019-06-22
author:	  yaowei.liu

----------------------------------------*/

#ifndef __SWITCH_H__
#define __SWITCH_H__

struct callback_func
{
	int (*get_input_year)(void);
	int (*calc_leap_year)(int);
};

int year;
int month;

#endif















