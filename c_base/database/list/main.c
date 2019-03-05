#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"

typedef struct
{
	int a;
	list_head head;
}data_t;

int main(int argc, char const *argv[])
{
	data_t *tmp;
	data_t data[10];
	list_head head;
	list_head *pos;
	int i;

	list_init(&head);

	for(i = 0; i < 10; i++) {
		data[i].a = i;
		//list_insert_after(&head, &(data[i].head));
		list_insert_before(&head, &(data[i].head));//fifo
	}

	list_for_each(pos, &head)
	{
		tmp = container_of(pos, data_t, head);
		printf("before delete: a = %d\n", tmp->a);
		sleep(1);
	}

	tmp = list_first_entry(pos, data_t, head);
	printf("first id: %d\n", tmp->a);
	list_delete_init(&tmp->head);

	list_for_each(pos, &head)
	{
		tmp = container_of(pos, data_t, head);
		printf("after delete: a = %d\n", tmp->a);
		sleep(1);
	}

	return 0;
}

