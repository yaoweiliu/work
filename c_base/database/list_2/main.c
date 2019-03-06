/* https://github.com/BiscuitOS/HardStack/blob/master/Algorithem/list/single-list/TripPoint-Single.c */

#include <stdio.h>
#include <stdlib.h>

struct node
{
	int a;
	struct node *next;
};

void insert(struct node ***list, struct node *data)
{
	**list = data;
	*list = &data->next;
	data->next = NULL;//需要设置data->next为NULL，否则37行for循环的第二个条件一直为真[野指针也有指向]
}

int main(int argc, char const *argv[])
{
	struct node data1, data2, data3, data4;
	struct node **data_list;
	struct node *tmp;

	data1.a = 0x1;
	data2.a = 0x2;
	data3.a = 0x3;
	data4.a = 0x4;

	data_list = &data1.next;

	insert(&data_list, &data1);//改变二级指针的指向，需要使用三级指针
	insert(&data_list, &data2);
	insert(&data_list, &data3);
	insert(&data_list, &data4);

	for(tmp=&data1; tmp; tmp=tmp->next)
		printf("data: %d\n", tmp->a);

	return 0;
}
