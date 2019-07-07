/*---------------------------------------------------
file: database.h
brief: student infomation and device fd etc.
date: 2019-07-05
---------------------------------------------------*/

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdio.h>
#include "list.h"

#define NAME_LEN	12

typedef struct 
{
	int fd;
	FILE *fp;
}FILE_FD;

typedef struct
{
	float math;
	float chinese;
	float english;
}GRADE;

typedef struct
{
	int id;
	char name[NAME_LEN];
	char sex[NAME_LEN];
	int age;
	GRADE grade;
	struct list_head stu_list;
}STUDENT;

#endif


