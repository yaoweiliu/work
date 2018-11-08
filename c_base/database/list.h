#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

struct list_node
{
	struct list_node *next;
	struct list_node *prev;
};

typedef struct list_node list_head;

int list_init(list_head *head);

int list_insert_after(list_head *head, list_head *node);

int list_delete(list_head *node);

//void list_display(list_head *head);
#define list_for_each(pos, head)	\
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#endif