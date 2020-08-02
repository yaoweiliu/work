#include <stdio.h>

struct list_data
{
	int value;
	struct list_data *next;
};

static struct list_data* reverseList(struct list_data *head)
{
	if(head == NULL)
		return NULL;

	struct list_data *curr = head;
	struct list_data *bk, *new = NULL;

	while(curr) {
		bk = curr->next;
		curr->next = new;
		new = curr;
		curr = bk;
	}

	return new;
}


