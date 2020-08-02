#include <stdio.h>

struct list_data
{
	int value;
	struct list_data *next;
};

static struct list_data *mergeList(struct list_data *list1, struct list_data *list2)
{
	if(list1 == NULL && list2 == NULL)
		return NULL;
	else if(list1 == NULL)
		return list2;
	else
		return list1;

	struct list_data *new;

	if(list1->value <= list2->value) {
		new = list1;
		new->next = mergeList(list1->next, list2)
	}
	else {
		new = list2;
		new->next = mergeList(list1, list2->next);
	}

	return new;
}


