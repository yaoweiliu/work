#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct list_node
{
	struct list_node *next;
	struct list_node *prev;
};

typedef struct list_node list_head;

int list_init(list_head *head);

//new node is inserted after head.
int list_insert_after(list_head *head, list_head *node);

//new node is inserted before head. fifo
int list_insert_before(list_head *head, list_head *node);

int list_delete_init(list_head *node);

#define list_first_entry(ptr, type, member)	\
	container_of((ptr)->next, type, member)

//void list_display(list_head *head);
#define list_for_each(pos, head)	\
	for (pos = (head)->next; pos != (head); pos = pos->next)

#endif