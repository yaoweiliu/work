/* https://github.com/BiscuitOS/HardStack/blob/master/Algorithem/list/single-list/TripPoint-Single.c */

#include <stdio.h>

struct node
{
	int a;
	struct node *next;
};

void insert(struct node ***list, struct node *data)
{

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

	insert(&data_list, &data1);

	return 0;
}