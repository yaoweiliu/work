/*---------------------------------------------------
file: display.h
brief: display infomation.
date: 2019-07-06
---------------------------------------------------*/
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "database.h"
#include <string.h>
#include <unistd.h>

void display_init(int num);

void display_add_stu(STUDENT *stu);

void display_error_info(void);

void dispaly_success_info(void);

void display_info(int sum);

#endif

