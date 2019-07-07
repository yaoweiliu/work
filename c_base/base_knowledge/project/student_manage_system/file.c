#include "file.h"
#include <stdio.h>
#include <string.h>

int open_file(FILE_FD fd)
{
	fd.fp = fopen("./stuent.data", "ab+");
	if(fd.fp == NULL) {
		fprintf(stderr, "%s: open file error\n", __func__);
		goto open_error;
	}

	return 0;

open_error:
	return -1;
}

int close_file(FILE_FD fd)
{
	return fclose(fd.fp);
}

int read_file(FILE_FD fd, STUDENT *stu)
{
	int ret;

	if(ret = open_file(fd)) {
		fprintf(stderr, "%s: open file error\n", __func__);
		goto open_error;
	}

	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	if(ret = fread((void *)stu, sizeof(STUDENT), 1, fd.fp) != 1) {
		fprintf(stderr, "%s: read file error or file is end\n", __func__);
		goto read_error;
	}

	close_file(fd);

	return 0;

read_error:
	close_file(fd);
open_error:
	return -1;
}

int write_file(FILE_FD fd, const STUDENT *stu)
{
	int ret;

	if(ret = open_file(fd)) {
		fprintf(stderr, "%s: open file error\n", __func__);
		goto open_error;
	}

	//size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
	if(ret = fwrite((void *)stu, sizeof(STUDENT), 1, fd.fp) != 1) {
		fprintf(stderr, "%s: write file error\n", __func__);
		goto write_error;
	}

	close_file(fd);

	return 0;

write_error:
	close_file(fd);
open_error:
	return -1;
}

void init_list(void)
{
	INIT_LIST_HEAD(&list);
}

STUDENT *find_stu(int id, STUDENT student)
{
	STUDENT *pos = NULL;

	list_for_each_entry(pos, &student.stu_list, stu_list) {
		if(pos->id == id)
			return pos;
	}

	return NULL;
}

int add_stu(STUDENT *stu)
{
	list_add_tail(&stu->stu_list, &list);

	return 0;
}

int remove_stu(int id, STUDENT student)
{
	STUDENT *pos = NULL;
	STUDENT *tmp = NULL;

	list_for_each_entry_safe(pos, tmp, &student.stu_list, stu_list) {
		if(pos->id == id) {
			list_del_init(&pos->stu_list);
			free(pos);
		}
	}

	return 0;
}

int modify_stu(int id, STUDENT stu, STUDENT student)
{
	STUDENT *pos = NULL;

	list_for_each_entry(pos, &student.stu_list, stu_list) {
		if(pos->id == id)
			memcpy(pos, &stu, sizeof(STUDENT));
	}
}

int display_all_stu(STUDENT *student)
{
	//STUDENT *pos = NULL;
	struct list_head *pos;
	STUDENT *tmp;

	//list_for_each_entry(pos, &student->stu_list, stu_list)
	list_for_each(pos, &list) {
		tmp = container_of(pos, STUDENT, stu_list);
		printf("%d\n", tmp->id);
	}
/*		
-----------------------------------------------------------------------
  学号  |  姓名  |  性别  |  年龄  |  数学  |  语文  |  英语  | 总成绩
--------+--------+--------+--------+--------+--------+--------+--------
   01   | l      |   女   |   21   | 99.00  | 99.00  | 99.00  | 297.00
   02   | 2      |   男   |   23   | 23.00  | 23.00  | 23.00  | 69.00
--------+--------+--------+--------+--------+--------+--------+--------
   --   |   --   |  1/1   | 22.00  | 61.00  | 61.00  | 61.00  | 183.00
-----------------------------------------------------------------------
*/
}

