#include "list.h"

int list_init(list_head *head)
{
	head->next = head;
	head->prev = head;
}

int list_insert_after(list_head *head, list_head *node)
{
	head->next->prev = node;
	node->next = head->next;

	node->prev = head;
	head->next = node;
}

int list_delete(list_head *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = node;
	node->prev = node;
}

#if 0
void list_display(list_head *head)
{

}
#endif
