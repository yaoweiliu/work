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

int list_insert_before(list_head *head, list_head *node)
{
	head->prev->next = node;
	node->prev = head->prev;

	node->next = head;
	head->prev = node;
}

int list_delete_init(list_head *node)
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
