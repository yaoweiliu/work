#include "database.h"

int main(int argc, char const *argv[])
{
	struct list_head head;
	struct database *data = NULL;
	//struct database data[5];
	int num, i;
	struct database find = {.id = 3};
	struct database *tmp = NULL;

	if(argc < 2) {
		printf("Usage is: ./main + num\n");
		return -1;
	}
	num = atoi(argv[1]);

	list_create_init(&head);
	for(i = 0; i < num; i++) {
		#if 1
		data = (struct database *)malloc(sizeof(struct database));
		if(!data)
			return -1;
		data->id = i;
		list_insert_tail(&head, data);
		#endif

		#if 0
		data[i].id = i;
		list_insert_tail(&head, &data[i]); //不能直接传递data[i]，要传递地址。\
		此处不能传递局部变量，局部变量的生命周期在函数调用完成就结束了，所以传递局部变量时地址会发生改变。
		#endif
	}

	list_display(&head);
	list_remove_first(&head);
	list_display(&head);

	tmp = list_find(&head, find);
	if(tmp)
		printf("find it: id is %d\n", tmp->id);
	list_modify_entry(&head, find, 10);
	list_display(&head);

	list_destroy(&head);
	list_display(&head);
	
	return 0;
}



