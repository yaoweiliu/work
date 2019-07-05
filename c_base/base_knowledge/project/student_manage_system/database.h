/*---------------------------------------------------
file: database.h
brief: device fd etc.
date: 2019-07-05
---------------------------------------------------*/

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdio.h>

typedef struct 
{
	int fd;
	FILE *fp;
}FILE_FD;

typedef struct
{
	char *name;
	int number;
	char *sale;
}STUDENT;

typedef struct
{
	float english;
	float math;
	float yuwen;
};



#endif


