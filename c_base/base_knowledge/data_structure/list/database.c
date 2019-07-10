#include "database.h"

void list_create_init(struct list_head *head)
{
	return INIT_LIST_HEAD(head);
}

void list_insert_tail(struct list_head *head, struct database *data)
{
	return list_add_tail(&data->list, head);
}

void list_remove_first(struct list_head *head)
{
	struct database *pos = NULL;
	if(!list_is_empty(head)) {
		pos = list_first_entry(head, struct database, list);
		//printf("first entry is: id %d ,address %p\n", pos->id, pos);
		list_del_init(&pos->list);
		free(pos);
	}
}

struct database *list_find(struct list_head *head, struct database data)
{
	struct list_head *pos = NULL;
	struct database *tmp = NULL;

	list_for_each(pos, head) {
		tmp = container_of(pos, struct database, list);
		if(tmp->id == data.id)
			return tmp;
	}
	return NULL;
}

int list_modify_entry(struct list_head *head, struct database data, int value)
{
	struct database *tmp = NULL;

	//list_replace_init();
	tmp = list_find(head, data);
	if(tmp) {
		tmp->id = value;
		return 0;
	}
	return -1;
}

int list_is_empty(struct list_head *head)
{
	return list_empty(head);
}

void list_display(struct list_head *head)
{
	//struct list_head *pos = NULL;
	struct database *tmp = NULL;

	#if 0
	list_for_each(pos, head) {
		tmp = container_of(pos, struct database, list);
		if(tmp)
			printf("id is %d, tmp is %p\n", tmp->id, tmp);
	}
	#endif
	list_for_each_entry(tmp, head, list) {
		printf("id is %d, tmp is %p\n", tmp->id, tmp);
		sleep(1);
	}
}

void list_destroy(struct list_head *head)
{
	struct list_head *pos = NULL;
	struct database *tmp = NULL;

	list_for_each(pos, head) {
		tmp = container_of(pos, struct database, list);
		if(tmp) {
			//list_del_init(&tmp->list);
			free(tmp);
			tmp = NULL;
		}
	}
}


