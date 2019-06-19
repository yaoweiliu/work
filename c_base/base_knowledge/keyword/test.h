/*------------------------------------
file:   test.h
brief:  extern keyword
author: yaowei.liu
date:   2019.6.19
 -------------------------------------*/

#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>

typedef enum
{
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
}LEVEL;

#define PRINT_LEVEL	LEVEL_INFO
#define MAXNUM		248

extern int send_event(int code);
extern int get_code(int idx);
extern void print(LEVEL level, char *info);
int store[MAXNUM];
int idx;

void add_code(int code);
void process_event(int code);

#endif

