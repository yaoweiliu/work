#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

int main(int argc, char const *argv[])
{
	DataNode_T *fifo = NULL;
	int i;

	fifo = malloc(sizeof(DataNode_T));

	fifo_init(fifo);

	for(i = 0; i < 1; i++) {
		DataNode_T *node = malloc(sizeof(DataNode_T));
		node->count = i+1;
		fifo_enqueue(fifo, node);
	}

	//DataNode_T *tmp = NULL;
	//i = fifo_dequeue(fifo, tmp);
	//printf("i = %d\n", i);

	fifo_display(fifo);

	//printf("count = %d\n", fifo->count);
	int *a = malloc(sizeof(int));

	printf("%p, %p\n", container_of(a, DataNode_T, count), a);

	return 0;
}