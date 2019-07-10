#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

struct database
{
	int id;
	struct list_head list;
};

void list_create_init(struct list_head *head);

void list_insert_tail(struct list_head *head, struct database *data);

void list_remove_first(struct list_head *head);

struct database *list_find(struct list_head *head, struct database data);

int list_modify_entry(struct list_head *head, struct database data, int value);

int list_is_empty(struct list_head *head);

void list_display(struct list_head *head);

void list_destroy(struct list_head *head);

#endif


