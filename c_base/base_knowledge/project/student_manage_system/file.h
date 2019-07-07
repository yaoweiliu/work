/*---------------------------------------------------
file: file.h
brief: store student infomation.
date: 2019-07-06
---------------------------------------------------*/

#ifndef __FILE_H__
#define __FILE_H__

#include "database.h"
#include <stdlib.h>

static struct  list_head list;

int open_file(FILE_FD fd);

int read_file(FILE_FD fd, STUDENT *stu);

int write_file(FILE_FD fd, const STUDENT *stu);

int close_file(FILE_FD fd);

void init_list(void);

STUDENT *find_stu(int id, STUDENT student);

int add_stu(STUDENT *stu);

int remove_stu(int id, STUDENT student);

int modify_stu(int id, STUDENT stu, STUDENT student);

int display_all_stu(STUDENT *student);

#endif



